Design a scalable data pipeline to ingest and process 10 TB of daily clickstream data from user devices. Describe the architecture, tools, and key considerations for reliability, scalability, and cost.

Tools 
- Azure Event Hubs for collecting data from clickstream
- Databricks hosted spark job for structured streaming
- Databricks Delta Table for storage
- Azure Data Lake Gen2 for mounting the unity catalog
- Azure monitor for monitoring

Architecture
- Azure Event hub subscribes data from clickstream source as topic
- Databricks hosted job read data from topic as structured streaming
- Databricks unity catalog is created in ADLS gen2
- Databricks catalog is created which is mounted on ADLS gen2 container
- Data is loaded incrementally
- data older than 1 year is moved into archive with a batch job

Functional Requirements
- Connect to clickstream datasource and read streaming data into event hubs
- Subscribe to event hub topic and load into delta table
- Load data into databricks delta table
- Archive 1 year old data
- Monitoring and Alerting for any failues

Non Functional Requirements
- Source should be available
- Data should be reliable
- Cost should be minimal
- Everything should be automated
- data loss should not occur
- data should not be duplicated.

Azure Event Hubs
- Subscribe to clickstream, picks up data in microbatch
- A topic created for this data
- Pyspark code connecting to this event hub for reading data
- Data retention of 2 days
- Azure monitor configured with alerts to team if any issues occur
- Azure monitor to check ingestion log for past 30 days

Databricks
- A unity catalog created on top of Azure data lake gen2, lets say storageA container unity-catalog
- A catalog created on top of ADLS gen2, lets say storageA container clickstream-logs
- Delta table for storing the logs in databricks
- A notebook created to read from Azure event hub
    - Transform data types
    - Merge into delta table as slowly changing dimension type 2
    - Load into delta table
- A separate notebook for
    - Reading logs older than 1 year from delta table
    - Load into a separate ADLS gen2 with archive tier , lets say StorageB container clickstream-logs-archive
    - Delete from delta table

Reliability Improvements
- Enable checkpointing in spark to avoid data loss
- Monitoring and alerting integration in event hubs
- Monitoring and alerting integration in databricks notebook for failures
- Retrigger mechanism for notebook
- Data type change handling by mentioning column names and typecasting

Scalability Improvements
- Azure Event Hubs on higher tier
- Databricks compute with autoscale
- Manage partitions in spark to haandle large volume of data and shuffling.


Question 1:

Your resume highlights an architecture migration from SAP OData to a REST API-based ingestion system, which achieved near-real-time data freshness and reduced extraction failures by 85%.

Can you walk me through the high-level architecture of this new system? Specifically, what components did you use in Azure for the ingestion, transformation, and storage layers, and how did you implement the improved error handling and retry mechanism that was so effective?

Functional Requirements
 - Data is to be fetched from SAP ODATA and loaded into power bi dashboard
 - There is no direct SAP connectivity to HANA 
 - SAP BTP integration API is available to consume data
 - Power BI dashboard shows transformed data
 - System should be available with data and option to refresh if not up to date

Non Functional Requirements
- Data is not realtime, it can be once in a day or on demand
- System should be available with data and option to refresh if not up to date
- Integration should work between components
- User should be able to see status of data load

Low Level Design
- SAP exposes BTP endpoint for ODATA views. It is https which can be accessed with username and password as credential for authentication.
- An Azure SQL database is used in a auto-pause mode and serverless compute tier to store the metadata like ODATA Name, Filter query in API, Page size, TOP, SKIP, Table Name, ChangeCaptureEnabled, ChangeCaptureColumn, ID, ActiveFlag
- Azure Data Factory is used for ingestion
    1. Linked service is created as REST with SAP BTP endpoint and username and password is consumed from Azure Key Vault for secure access.
    2. A dataset in ADF is created with parameterized ODATA view name and appropriate filter condition
    3. A second dataset is created to consume from SQL database for metadata
    4. In ingestion pipeline, parameterized variables like ODATA Name, ID is used to lookup into SQL database and fetch the appropriate view and filter condition. It can be full load or Incremental Load ( SCD type 1 ) preferably
    5. Based on lookup output, Call is made to SAP API with parameters and stored in azure blob storage.
- Frontend ( Power BI )
    1. A Power BI Dataflow is created on top of Azure blob storage which will do the defined transformations and load in Power BI
    2. Power BI dataset is created on top of dataflow which powers the report
    3. Power BI report is built which is open for end users
    4. Report has a button for 'Refresh Now'. Report also has timestamp to which point report is refreshed.
    5. If it is not refreshed from last 30 minutes, button is enabled.
- On Demand Refresh 
    1. Button click triggers an Azure Function which has series of APIs
    2. API 1 is Trigger ADF pipeline which is already parameterized. In this case, trigger picks up input from Power BI selection. This constantly monitors run every 10 seconds and updates in DB if state changes.
    3. API 2 is Trigger Power BI dataflow refresh, monitor continuously
    4. API 3 is Trigger Power BI dataset
    5. API to refresh report.

- Error Handling
1. ADF pipeline has retry mechanism for 3 on critical activities
2. If failure occurs at any point, it is updated in DB with timestamp and notified to user on Power BI report.
3. Engineering team gets an email for aany error and associated link and document to fix
4. To avoid access issues and dependency, managed identity in azure is used to avoid passwords
5. To avoid data duplication, backup is created before inserting data and checked for duplicated. If found, revert to old state and notify.
6. In case of any primary key violation, that records are excluded and notified.


Can you explain specifically why you would choose Delta Lake over standard Parquet files in Blob Storage for this architecture?

Please detail:

How Delta Lake handles ACID transactions (specifically in the context of your "retry mechanisms" and preventing partial loads).

How you would structure the partitioning strategy for a high-volume table (e.g., 500M rows) to optimize query performance?

How does V-Order or Z-Ordering play a role here?