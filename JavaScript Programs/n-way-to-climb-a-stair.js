countWays = (n) => {
    if(n === 0) {
        return 1;
    } 
    if(n < 0) {
        return 0;
    }
    return countWays(n - 1) + countWays(n - 2);
}

console.log(countWays(10));