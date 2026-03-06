<#
.SYNOPSIS
    Identifies which code lines in Azure Automation runbooks use Automation variables.

.DESCRIPTION
    Scans multiple Azure Automation accounts for PowerShell and Python runbooks,
    then reports each line that references Azure Automation variables.

.PARAMETER SubscriptionId
    Optional. Specific subscription ID to scan. If not provided, scans all accessible subscriptions.

.PARAMETER ResourceGroup
    Optional. Filter by resource group name.

.PARAMETER AutomationAccountName
    Optional. Specific automation account name. If not provided, scans all accounts.

.PARAMETER OutputFormat
    Table, CSV, or Json. Default: Table

.PARAMETER LocalPath
    Optional. Scan local .ps1/.py files instead of Azure. Path to folder or specific file.

.EXAMPLE
    .\Find-AutomationVariableUsage.ps1
    # Scans all Automation accounts in current subscription

.EXAMPLE
    .\Find-AutomationVariableUsage.ps1 -SubscriptionId "xxx" -OutputFormat CSV -OutputPath results.csv

.EXAMPLE
    .\Find-AutomationVariableUsage.ps1 -LocalPath "C:\ExportedRunbooks"
    # Scans local PowerShell and Python files
#>

[CmdletBinding()]
param(
    [Parameter()]
    [string]$SubscriptionId,

    [Parameter()]
    [string]$ResourceGroup,

    [Parameter()]
    [string]$AutomationAccountName,

    [Parameter()]
    [ValidateSet('Table', 'CSV', 'Json')]
    [string]$OutputFormat = 'Table',

    [Parameter()]
    [string]$OutputPath,

    [Parameter()]
    [string]$LocalPath
)

# Patterns that indicate Azure Automation variable usage
$VariablePatterns = @{
    PowerShell = @(
        # Get-AutomationVariable cmdlet (all variations)
        'Get-AutomationVariable',
        # Legacy/internal patterns
        'Get-AutomationConnection',
        'Get-AutomationCertificate',
        'Get-AutomationCredential',
        'Get-AutomationPSCredential',
        'Get-AutomationSchedule'
    )
    Python = @(
        'automationassets\.get_automation_variable',
        'automationassets\.get_automation_connection',
        'automationassets\.get_automation_credential',
        'get_automation_variable',
        'get_automation_connection',
        'get_automation_credential'
    )
}

# Combined regex for line matching (case-insensitive)
$PsPattern = '(' + ($VariablePatterns.PowerShell -join '|') + ')'
$PyPattern = '(' + ($VariablePatterns.Python -join '|') + ')'

function Get-AutomationAccounts {
    param([string]$SubId, [string]$RG, [string]$AccName)

    $params = @{}
    if ($SubId) {
        $params['SubscriptionId'] = $SubId
    }
    if ($RG) {
        $params['ResourceGroupName'] = $RG
    }
    if ($AccName) {
        $params['Name'] = $AccName
    }

    Get-AzAutomationAccount @params -ErrorAction SilentlyContinue
}

function Get-RunbookContent {
    param(
        [string]$ResourceGroupName,
        [string]$AutomationAccountName,
        [string]$RunbookName,
        [string]$RunbookType
    )

    try {
        $content = Export-AzAutomationRunbook `
            -ResourceGroupName $ResourceGroupName `
            -AutomationAccountName $AutomationAccountName `
            -Name $RunbookName `
            -OutputFolder $env:TEMP `
            -Force `
            -ErrorAction Stop

        $ext = switch ($RunbookType) {
            'PowerShell' { '.ps1' }
            'PowerShell7' { '.ps1' }
            'Python2' { '.py' }
            'Python3' { '.py' }
            'GraphPowerShell' { '.ps1' }
            default { '.ps1' }
        }

        $filePath = Join-Path $env:TEMP "$RunbookName$ext"
        if (Test-Path $filePath) {
            Get-Content $filePath -Raw
        } else {
            $null
        }
    } catch {
        Write-Warning "Failed to export runbook '$RunbookName': $_"
        $null
    } finally {
        $tempFile = Join-Path $env:TEMP "$RunbookName*.ps1"
        Remove-Item $tempFile -Force -ErrorAction SilentlyContinue
        $tempFile = Join-Path $env:TEMP "$RunbookName*.py"
        Remove-Item $tempFile -Force -ErrorAction SilentlyContinue
    }
}

function Find-VariableUsage {
    param(
        [string]$Content,
        [string]$RunbookType
    )

    if ([string]::IsNullOrWhiteSpace($Content)) {
        return @()
    }

    $pattern = if ($RunbookType -match 'Python') { $PyPattern } else { $PsPattern }
    $lines = $Content -split "`r?`n"
    $results = @()
    $lineNum = 0

    foreach ($line in $lines) {
        $lineNum++
        if ($line -match $pattern) {
            $results += [PSCustomObject]@{
                LineNumber = $lineNum
                LineContent = $line.Trim()
                MatchedPattern = $Matches[1]
            }
        }
    }

    $results
}

$allResults = @()

if ($LocalPath) {
    # ========== LOCAL FILE SCAN ==========
    if (-not (Test-Path $LocalPath)) {
        Write-Error "LocalPath not found: $LocalPath"
        exit 1
    }
    $files = if (Test-Path $LocalPath -PathType Leaf) {
        Get-Item $LocalPath
    } else {
        Get-ChildItem -Path $LocalPath -Recurse -Include *.ps1,*.py -File -ErrorAction SilentlyContinue
    }
    foreach ($file in $files) {
        $content = Get-Content $file.FullName -Raw -ErrorAction SilentlyContinue
        $runbookType = if ($file.Extension -eq '.py') { 'Python3' } else { 'PowerShell' }
        $usages = Find-VariableUsage -Content $content -RunbookType $runbookType
        foreach ($u in $usages) {
            $allResults += [PSCustomObject]@{
                Subscription      = 'Local'
                SubscriptionId    = ''
                ResourceGroup     = $file.DirectoryName
                AutomationAccount = 'Local'
                RunbookName       = $file.Name
                RunbookType       = $runbookType
                LineNumber        = $u.LineNumber
                LineContent       = $u.LineContent
                MatchedPattern    = $u.MatchedPattern
            }
        }
    }
} else {
    # ========== AZURE SCAN ==========
    $requiredModules = @('Az.Accounts', 'Az.Automation')
    foreach ($mod in $requiredModules) {
        if (-not (Get-Module -ListAvailable -Name $mod)) {
            Write-Error "Required module '$mod' not found. Install with: Install-Module $mod -Scope CurrentUser"
            exit 1
        }
    }
    $context = Get-AzContext -ErrorAction SilentlyContinue
    if (-not $context) {
        Connect-AzAccount
    }
    $accounts = Get-AutomationAccounts -SubId $SubscriptionId -RG $ResourceGroup -AccName $AutomationAccountName
    if (-not $accounts) {
        Write-Warning "No Automation accounts found. Check subscription, resource group, and account name filters."
        exit 0
    }
    foreach ($account in $accounts) {
    $rgName = $account.ResourceGroupName
    $accName = $account.AutomationAccountName
    $subName = (Get-AzSubscription -SubscriptionId $account.SubscriptionId -ErrorAction SilentlyContinue).Name

    Write-Verbose "Scanning: $accName (RG: $rgName)"

    $runbooks = Get-AzAutomationRunbook -ResourceGroupName $rgName -AutomationAccountName $accName -ErrorAction SilentlyContinue
    if (-not $runbooks) {
        continue
    }

    foreach ($rb in $runbooks) {
        # Skip draft-only if we want only published; Export gets published by default
        $content = Get-RunbookContent -ResourceGroupName $rgName -AutomationAccountName $accName -RunbookName $rb.Name -RunbookType $rb.Type
        $usages = Find-VariableUsage -Content $content -RunbookType $rb.Type

        foreach ($u in $usages) {
            $allResults += [PSCustomObject]@{
                Subscription    = $subName
                SubscriptionId  = $account.SubscriptionId
                ResourceGroup   = $rgName
                AutomationAccount = $accName
                RunbookName     = $rb.Name
                RunbookType     = $rb.Type
                LineNumber      = $u.LineNumber
                LineContent     = $u.LineContent
                MatchedPattern  = $u.MatchedPattern
            }
        }
    }
    }
}

# Output
if ($allResults.Count -eq 0) {
    Write-Host "No Azure Automation variable usage found in scanned runbooks." -ForegroundColor Yellow
    exit 0
}

$accountCount = if ($LocalPath) { 1 } else { $accounts.Count }
Write-Host "Found $($allResults.Count) line(s) using Automation variables." -ForegroundColor Green

if ($OutputPath) {
    switch ($OutputFormat) {
        'CSV'  { $allResults | Export-Csv -Path $OutputPath -NoTypeInformation -Encoding UTF8 }
        'Json' { $allResults | ConvertTo-Json -Depth 5 | Set-Content -Path $OutputPath -Encoding UTF8 }
        default { $allResults | Export-Csv -Path ($OutputPath -replace '\.\w+$', '.csv') -NoTypeInformation -Encoding UTF8 }
    }
    Write-Host "Results saved to: $OutputPath"
}

switch ($OutputFormat) {
    'CSV'  { if (-not $OutputPath) { $allResults | ConvertTo-Csv -NoTypeInformation } }
    'Json' { if (-not $OutputPath) { $allResults | ConvertTo-Json -Depth 5 } }
    default { $allResults | Format-Table -AutoSize -Wrap Subscription, AutomationAccount, RunbookName, RunbookType, LineNumber, MatchedPattern, LineContent }
}
