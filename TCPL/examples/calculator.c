#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include "stack.h"

#define	BUF_SIZE	100	/* max size of operand or operator */
#define	NUMBER		'0'	/* signal that a number was found */
#define	MATH		'F'	/* signal that a function in math.h was found */

struct one_op_func_t {
	const char *name;
	double (*func)(double);
};
struct two_op_func_t {
	const char *name;
	double (*func)(double, double);
};

static struct one_op_func_t funcs[] = {
	{"sin", sin},
	{"cos", cos},
	{"tan", tan},
	{"exp", exp},
	{"log", log},
	{NULL, NULL}
};

static struct two_op_func_t funcs2[] = {
	{"pow", pow},
	{NULL, NULL}
};

void math_func(char *name)
{
	int i;

//	for (i = 0; funcs[i].name != NULL; i++) {
//		if (!strncmp(name, funcs[i].name, strlen(funcs[i].name)))
//			break;
//	}
//
//	if (funcs[i].name != NULL) {
//		push(funcs[i].func(pop()));
//		return;
//	}
//
//	for (i = 0; funcs2[i].name != NULL; i++) {
//		if (!strncmp(name, funcs2[i].name, strlen(funcs2[i].name)))
//			break;
//	}
//
//	if (funcs2[i].name != NULL) {
//		double op1 = pop();
//		push(funcs2[i].func(op1, pop()));
//		return;
//	}
}

int getop(char *s);
int getch(void);
void ungetch(int c);

static struct stack_t char_stack;
int main(int argc, char *argv[])
{
	int type;
	char buf[BUF_SIZE];
	struct stack_t *iobuf = &char_stack;
	double op2;

	stack_init(iobuf, NULL);

	while ((type = getop(buf)) != EOF) {
//		switch (type) {
//		case MATH:
//			math_func(buf);
//			break;
//		case NUMBER:
//			push(atof(buf));
//			break;
//		case '+':
//			push(pop() + pop());
//			break;
//		case '*':
//			push(pop() * pop());
//			break;
//		case '-':
//			op2 = pop();
//			push(pop() - op2);
//			break;
//		case '/':
//			op2 = pop();
//			if (op2 != 0.0)
//				push(pop() / op2);
//			else
//				printf("error: zero divisor\n");
//			break;
//		case '%':
//			op2 = pop();
//			if (op2 != 0.0) {
//				int op1 = (int) pop();
//				push((double)(op1 % ((int) op2)));
//			} else
//				printf("error: zero divisor\n");
//			break;
//		case '\n':
//			printf("\t%.8g\n", pop());
//			break;
//		default:
//			fprintf(stderr, "error: unknow command %s\n", buf);
//			break;
//		}
	}

	return 0;
}

int getop(char *s)
{
	int i = 0, c;

	while (isspace(c = getch()) && c !='\n')
		;
	s[0] = c;
	s[1] = '\0';

	if (c == '-') {
		c = getch();
		if (!isdigit(c)) {
			ungetch(c);
			c = s[0];
		} else
			s[++i] = c;
	}

	if (isalpha(c)) {
		while (isalpha(s[++i] = c = getch()))
			;
		s[i] = '\0';

		if (c != EOF)
			ungetch(c);
		return MATH;
	}

	if (!isdigit(c) && c != '.') {
		return c;
	}

	if (isdigit(c)) {
		while (isdigit(s[++i] = c = getch()))
			;
	}

	if (c == '.') {
		while (isdigit(s[++i] = c = getch()))
			;
	}
	s[i] = '\0';

	if (c != EOF)
		ungetch(c);

	return NUMBER;
}

int getch(void)
{
	int c = -1;
	struct stack_t *s = &char_stack;
	int *data;

	if (stack_size(s) > 0) {
		stack_pop(s, (void **) &data);
		c = *data;
	} else 
		c = getchar();

	return c;
}

void ungetch(int c)
{
	struct stack_t *s = &char_stack;
	int *data;

	data = (int *) malloc(sizeof(int));
	stack_push(s, data);
}

