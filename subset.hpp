//enumerate size k subsets
int comb = (1 << k) - 1;
while (comb < 1 << n) {
    
    int x = comb & -comb, y = comb + x;
    comb = ((comb & ~y) / x >> 1) | y;
}

// subsets of mask
// not evalate i == 0
for (int i = mask; i > 0; i = (i-1) & mask) {

}
//evalate i == 0
for (int i = mask; ; i = (i-1) & mask) {

    if (i == 0) break;
}

//subsets including mask
for (int i = mask; i < (1<<n); i = (i+1) | mask) {

}
