#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void update_merged(int *merged, int *cur_ind, const int *half, int *half_ind) {
    merged[*cur_ind] = half[*half_ind];
    (*cur_ind)++;
    (*half_ind)++;
}

int *merge(const int fst_half[], const int fst_len,
	   const int snd_half[], const int snd_len) {
    int *merged = calloc(fst_len + snd_len, sizeof(int));
    int fst_ind = 0;
    int snd_ind = 0;
    int cur_ind = 0;
    while (fst_ind < fst_len || snd_ind < snd_len) {
	if (fst_ind == fst_len) {
	    for (int i = snd_ind; i < snd_len; ++i) {
		update_merged(merged, &cur_ind, snd_half, &snd_ind);
	    }
	} else if (snd_ind == snd_len) {
	    for (int i = fst_ind; i < fst_len; ++i) {
		update_merged(merged, &cur_ind, fst_half, &fst_ind);
	    }
	} else if (fst_half[fst_ind] <= snd_half[snd_ind]) {
	    update_merged(merged, &cur_ind, fst_half, &fst_ind);
	} else {
	    update_merged(merged, &cur_ind, snd_half, &snd_ind);
	}
    }
    return merged;
}

int FORK_COUNTER = 0;

void merge_sort(int arr[], const int len) {
    if (len == 1) {
	return;
    } else {
	const int mid = len / 2;
	const int len_fst = len - mid;
	const int len_snd = mid;
	// If forked too often, it gets way too slow. (limited) testing showed that 5 is one of the fastest
	if (FORK_COUNTER < 5) {
	    pid_t pid = fork();
	    FORK_COUNTER++;
	    if (pid == 0) { // Child process
		merge_sort(arr, len_fst);
		exit(0);
	    } else { // Parent process
		merge_sort(arr+len_fst, len_snd);
		waitpid(pid, NULL, 0);
	    }
	} else {
	    merge_sort(arr, len_fst);
	    merge_sort(arr+len_fst, len_snd);
	}

	memcpy(arr, merge(arr, len_fst, arr+len_fst, len_snd),
	       len * sizeof(int));
    }
}

int main(int argc, char **argv) {
    // int arr[] = {2, 15, 91, 2, 23, 54, 125, 21, 52};
    // create file descriptor for mmap
    int *arr = mmap(NULL, 1000000 * sizeof(int), PROT_READ | PROT_WRITE,
		      MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    for (int i = 0; i < 1000000; ++i) {
	arr[i] = 1284123 - i;
    }
    merge_sort(arr, 1000000);
    for (int i = 0; i < 10; ++i) {
	printf("%d\n", arr[i]);
    }
    return 0;
}
