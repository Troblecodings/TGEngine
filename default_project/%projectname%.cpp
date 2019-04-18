#include "%projectname%.hpp"

int main() {
	AppPro editor = AppPro();

	initTGEngine(&editor.main_window, nullptr, &init);
	return 0;
}

void init() {

}