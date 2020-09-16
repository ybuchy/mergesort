#include <stdio.h>
#include <stdlib.h>

int *merge(const int fst_half[], const int fst_len,
	   const int snd_half[], const int snd_len) {
    int *merged = calloc(fst_len + snd_len, sizeof(int));
    int fst_ind = 0;
    int snd_ind = 0;
    int cur_ind = 0;
    while (fst_ind < fst_len || snd_ind < snd_len) {
	if (fst_ind == fst_len) {
	    for (int i = snd_ind; i < snd_len; ++i) {
		merged[cur_ind] = snd_half[snd_ind];
		cur_ind++;
		snd_ind++;
	    }
	} else if (snd_ind == snd_len) {
	    for (int i = fst_ind; i < fst_len; ++i) {
		merged[cur_ind] = fst_half[fst_ind];
		cur_ind++;
		fst_ind++;
	    }
	} else if (fst_half[fst_ind] <= snd_half[snd_ind]) {
	    merged[cur_ind] = fst_half[fst_ind];
	    cur_ind++;
	    fst_ind++;
	} else {
	    merged[cur_ind] = snd_half[snd_ind];
	    cur_ind++;
	    snd_ind++;
	}
    }
    return merged;
}

int *merge_sort(int arr[], const int len) {
    if (len == 1) {
	return arr;
    } else {
	const int mid = len / 2;
	const int len_fst = len - mid;
	const int len_snd = mid;
	int *fst_half = merge_sort(arr, len_fst);
	int *snd_half = merge_sort(arr+len_fst, len_snd);
	return merge(fst_half, len_fst, snd_half, len_snd);
    }
}

int main(int argc, char **argv) {
    int arr[] = {2, 15, 91, 2, 23, 54, 125, 21, 52};
    int *sorted = merge_sort(arr, 9);
    for (int i = 0; i < 9; ++i) {
	printf("%d\n", sorted[i]);
    }
    return 0;
}
