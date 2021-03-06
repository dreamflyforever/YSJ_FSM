/*
 * It's a part of smartfsm, by Shanjin Yang.
 * Email: sjyangv0@gmail.com
 *
 */

#include "fsm.h"
#include <string.h>

FSM *fsm_obj;
STATE_DIS *state_obj;
STATE_DIS *state_obj_copy;

FSM fsm_default = 
{
	0, NULL, "state_default",
};

int fsm_init()
{
	state_obj = (STATE_DIS *)malloc(sizeof(STATE_DIS));
	if (state_obj == NULL) {
		printf("malloc error: %s %d\n", __FILE__, __LINE__);
		return 0;
	}

	fsm_obj = (FSM *)malloc(sizeof(FSM));
	if (fsm_obj == NULL) {
		printf("malloc error: %s %d\n", __FILE__, __LINE__);
		return 0;
	}

	return 0;
}

int state_init(int state, FUNC_PTR func, uchar *name)
{
	fsm_init();

	if ((state_obj == NULL) || ( fsm_obj == NULL)) 
		ERROR_PRINTF;

	fsm_obj->state = state;
	fsm_obj->func = func;
	fsm_obj->name = name;

	state_obj->state = state;
	state_obj->func  = func;
	state_obj->name  = name;

	state_obj_copy = state_obj;
	list_init(&(state_obj->node));
	return 0;
}

int state_add(int state, FUNC_PTR func, uchar *name)
{
	/*Just make a symbol for state name*/
	int token = (int)*name;

	STATE_OBJ_MALLOC_AND_INSERT(token, state, func, name);

	state_obj = (STATE_DIS *)(&(state_obj->node))->next;
	return 0;
}

int state_remove(int state)
{
	STATE_DIS *tmp = state_obj_copy;
	while (!is_list_empty(&(tmp->node))) {
		if (tmp->state == state) {
			list_delete(&(tmp->node));
			tmp->state = 10000;
			tmp->func = NULL;
			tmp->name = "";
			return 0;
		}

		tmp = (STATE_DIS *)(&(tmp->node))->next;
	}

	if (tmp->state == state) {
		list_delete(&(tmp->node)); 

		tmp->state = 10000;
		tmp->func = NULL;
		tmp->name = "";

		return 0;
	}
	printf("State no fund\n");
	return 0;
}

int state_tran(int state, void *message)
{
	STATE_DIS *tmp = state_obj_copy;

	memset(fsm_obj->message, 0, MSG_LEN);
	while (!is_list_empty(&(tmp->node))) {
		if (tmp->state == state) {
			fsm_obj->func = tmp->func;
			fsm_obj->name = tmp->name; 
			fsm_obj->state = state;
			if (message != NULL)
				memcpy(fsm_obj->message, message, strlen((char *)message));

			return 0; 
		}

		tmp = (STATE_DIS *)(&(tmp->node))->next;
	}

	if (tmp->state == state) {
		fsm_obj->name = tmp->name; 
		fsm_obj->state = state;

		fsm_obj->func = tmp->func; 
		if (message != NULL)
			memcpy(fsm_obj->message, message, strlen((char *)message));
		return 0; 
	}
	state_default(fsm_default.state, fsm_default.func, fsm_default.name);

	printf("state is no found, and fsm will transfer to default, It is name: %s\n", fsm_obj->name);
	return 0;
}

void fsm_while(FSM *obj)
{
	while (1) {
		printf("state name = %s\n", obj->name);
		obj->func(obj->message);
	}
}

int state_default(int state, FUNC_PTR func, uchar *name)
{
	fsm_obj->state = state;
	fsm_obj->func  = func;
	fsm_obj->name  = name;

	return 0;
}
