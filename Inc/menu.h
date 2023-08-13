#ifndef MENU_H_
#define MENU_H_

typedef struct menu_struct menu_t;

struct menu_struct {

	const char * name;
	menu_t * next;
	menu_t * prev;
	menu_t * child;
	menu_t * parent;
	void (*menu_function)(void);

};

extern menu_t menu1;
extern menu_t sub_menu1_1;
extern menu_t sub_menu1_1_1;
extern menu_t sub_menu1_2;
extern menu_t menu2;
extern menu_t sub_menu2_1;
extern menu_t sub_menu2_2;
extern menu_t sub_menu2_3;
extern menu_t sub_menu2_4;
extern menu_t menu3;
extern menu_t menu4;
extern menu_t menu5;




void menu_refresh(void);
void menu_next(void);
void menu_prev(void);
void menu_enter(void);
void menu_back(void);

#endif /* MENU_H_ */
