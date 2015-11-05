#include "common.h"

/* N is the child of P whose height increases by 1 */
do {
	/* balance_factor(P) has not yet been updated! */
	if (N == left_child(P)) {
		/* the left subtree increases */
		if (balance_factor(P) == 1) {
			/* rebalancing is required */
			if (balance_factor(N) == -1) {
				/* Left Right Case -> Reduce to Left Left Case */
				rotate_left(N);
			}
			rotate_right(P);
			break;
		} else if (balance_factor(P) == -1) {
			balance_factor(P) = 0;
			break;
		} else {
			balance_factor(P) = 1;
		}
	} else {
		/* the right subtree increases */
		if (balance_factor(P) == -1) {
			/* rebalancing is required */
			if (balance_factor(N) == 1) {
				/* Right Left Case -> Reduce to Right Right Case */
				rotate_right(N);
			}
			rotate_left(P);
			break;
		} else if (balance_factor(P) == 1) {
			balance_factor(P) = 0;
			break;
		} else {
			balance_factor(P) = -1;
		}
	}

	N = P;
	P = parent(N);
} while (P != NULL);
