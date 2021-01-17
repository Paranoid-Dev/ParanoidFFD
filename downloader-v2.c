#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* chapter[2048];
char *title, *author, *summary, *info, *chapterlist;
PyObject *mainModule;
int i;

int download () {
	PyRun_SimpleString("options = uc.ChromeOptions()");
	PyRun_SimpleString("options.headless = True");
	PyRun_SimpleString("options.add_argument('--headless')");
	PyRun_SimpleString("chrome = uc.Chrome(options=options)");
	PyRun_SimpleString("chrome.get(url)");
	PyRun_SimpleString("chapter = chrome.find_element_by_xpath('//*[@id=\"storytext\"]').text");

	i = i + 1;

	PyObject *chapterPy = PyObject_GetAttrString(mainModule, "chapter");
	chapter[i] = PyUnicode_AsUTF8(chapterPy);
//  printf("%s", chapter[i]);
  
}

int description () {
	PyRun_SimpleString("options = uc.ChromeOptions()");
	PyRun_SimpleString("options.headless = True");
	PyRun_SimpleString("options.add_argument('--headless')");
	PyRun_SimpleString("chrome = uc.Chrome(options=options)");
	PyRun_SimpleString("chrome.get(url)");
	PyRun_SimpleString("title = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/b').text");
	PyRun_SimpleString("author = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/a[1]').text");
	PyRun_SimpleString("summary = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/div').text");
	PyRun_SimpleString("try: info = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span[4]').text\nexcept: info = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span[3]').text");
	PyRun_SimpleString("chapterlist = chrome.find_element_by_xpath('//*[@id=\"chap_select\"]').text");
	PyRun_SimpleString("chrome.quit()");
	
	PyObject *titlePy = PyObject_GetAttrString(mainModule, "title");
	title = PyUnicode_AsUTF8(titlePy);										//title
	PyObject *authorPy = PyObject_GetAttrString(mainModule, "author");
	author = PyUnicode_AsUTF8(authorPy);									//author
	PyObject *summaryPy = PyObject_GetAttrString(mainModule, "summary");
	summary = PyUnicode_AsUTF8(summaryPy);									//summary
	PyObject *infoPy = PyObject_GetAttrString(mainModule, "info");
	info = PyUnicode_AsUTF8(infoPy);										//info
	PyObject *chapterlistPy = PyObject_GetAttrString(mainModule, "chapterlist");
	chapterlist = PyUnicode_AsUTF8(chapterlistPy);						//chapter-list
}

int next () {
	PyRun_SimpleString("try: next = chrome.find_element_by_xpath('//*[@id=\"content_wrapper_inner\"]/span/button[2]')\nexcept: next = chrome.find_element_by_xpath('//*[@id=\"content_wrapper_inner\"]/span/button')");
	PyRun_SimpleString("next.click()");
	PyRun_SimpleString("url = chrome.current_url");
	PyRun_SimpleString("chrome.quit()");
}

int print () {
	i = i - 1;
	//printf("i:%d", i);
	int a = 1;
	printf("%s\n", title);
	printf("By %s\n\n", author);
	printf("%s\n\n", summary);
	printf("%s\n\n\n", info);
	printf("Chapters\n");
	printf("%s\n\n\n", chapterlist);
	while (i > 0) {
		printf("Chapter %d : \n\n", a); //should change
		printf("%s\n\n", chapter[a]);
		a = a + 1;
		i = i - 1;
	}
}

int help () {
	printf("                Paranoid FanFiction.net Downloader v0.1.1  by Paranoid-Dev                \n");
	printf(" ________________________________________________________________________________________ \n");
	printf("                                                                                          \n");
	printf("Usage : ./ParanoidFFD <output file name(.txt)> <www.fanfiction.net url>                   \n");
	printf("                                                                                          \n");
	printf("        echo <www.fanfiction.net url> | ./ParanoidFFD -p : print fanfiction in terminal   \n");	
	printf("        ./ParanoidFFD --version : show ParanoidFFD version                                \n");
	printf("        ./ParanoidFFD -h , --help : show this page                                        \n");
}

int main (int argc, char *argv[]) {

	if (argc > 1) {
		
		if (strcmp(argv[1], "--version") == 0) {
			printf("Paranoid FanFiction.net Downloader v0.1.1 \n");
		}
		else if (strcmp(argv[1], "--help") == 0) {
			help ();
		}
		else if (strcmp(argv[1], "-h") == 0) {
			help ();
		}
		else if (strcmp(argv[1], "-p") == 0) { //downloader
			Py_Initialize();
			PyRun_SimpleString("import undetected_chromedriver as uc");
			mainModule = PyImport_AddModule("__main__");

			PyRun_SimpleString("url = input()");
			description ();
			
			i = 0;
			
			download ();
			next ();
			download ();
			next ();
			while (1) {
				download ();
				//if chapter[i-2] == chapter[i]; break; Py_Finalize(); print ();
				if (strcmp(chapter[i-2], chapter[i]) == 0) {
					print ();
					break;
				}
				next ();
			}
		}
		else {
			printf("saving %s to %s.txt\n",argv[2],argv[1]);
			size_t l;
			l = strlen(argv[1]) + strlen(argv[2]) + 43;
			char buf[l];
			sprintf(buf, "echo \"%s\" | ./ParanoidFFD -p > \"%s.txt\"",argv[2],argv[1]);
			system(buf);
		}
	}
	
	return 0;
}