findMaxOfThree = (x, y, z) => {
    if(x > y && x > z) {
        return x;    
    }
    else if(y > x && y > z) {
        return y;
    }
    else {
        return z;    
    }
}

findMaximumZeroSubarray = (arr, type1, type2) => {
    let brr, crr;
    let i, k;
    let r1, r2;
    for(i = 0; i < arr.length; i++) {
        if(arr[i] === 0) {
            break;
        }
    }
    k = 0;
    while(arr[i] === 0) {
        k++;
        i++;
    }
    if(k === arr.length) {
        console.log("true on :", arr);
        return k;
    }
    if(type1 === 0 && type2 === 0) {
        return k;
    }
    r1 = -1;
    r2 = -1;
    console.log(arr, type1, type2);
    if(type1 > 0) {
        brr = [...arr];
        for(i = 0; i < brr.length; i++) {
            if(brr[i] !== 0) {
                break;
            }
        }
        brr[i] = brr[i] - 1;
        r1 = findMaximumZeroSubarray(brr, (type1 - 1), type2);
    }
    if(type2 > 0) {
        crr = [...arr];
        for(i = 0; i < crr.length; i++) {
            if(crr[i] != 0) {
                break;
            }
        }
        crr[i] = 0;
        r2 = findMaximumZeroSubarray(crr, type1, (type2 - 1));
    }
    return findMaxOfThree(k, r1, r2);
}

temparr = [3, 4, 2, 0, 1];
t1 = 4;
t2 = 2;
console.log(findMaximumZeroSubarray(temparr, t1, t2));
/*
process.stdin.resume();
process.stdin.setEncoding("utf-8");

var stdin_input = "";
process.stdin.on("data",function(input){
    stdin_input += input;
});
process.stdin.on("end",function (){
    main(stdin_input);
});
function main(input){
    lines = input.split('\n');
    testcases = parseInt(lines[0]);
    inp = lines[1].split(' ');
    n = parseInt(inp[0]);
    t1 = parseInt(inp[1]);
    t2 = parseInt(inp[2]);
    array = [];
    val = lines[2].split(' ');
    let i = 0;
    for(i = 0; i < n; i++) {
        array.push(parseInt(val[i]));
    }
    console.log(findMaximumZeroSubarray(array, t1, t2));

}
*/