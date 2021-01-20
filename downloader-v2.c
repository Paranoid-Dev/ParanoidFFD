#include <Python.h>	//	>=Python3.6
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* chapter[2048];
char* chaptername[2048];
char *title, *author, *summary, *info;
PyObject *mainModule;
int i, j;

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
	PyRun_SimpleString("try: chapterlist = chrome.find_element_by_xpath('//*[@id=\"chap_select\"]').text\nexcept: chapterlist = title");
	PyRun_SimpleString("chrome.quit()");
	
	PyObject *titlePy = PyObject_GetAttrString(mainModule, "title");
	title = PyUnicode_AsUTF8(titlePy);												//title
	PyObject *authorPy = PyObject_GetAttrString(mainModule, "author");
	author = PyUnicode_AsUTF8(authorPy);											//author
	PyObject *summaryPy = PyObject_GetAttrString(mainModule, "summary");
	summary = PyUnicode_AsUTF8(summaryPy);											//summary
	PyObject *infoPy = PyObject_GetAttrString(mainModule, "info");
	info = PyUnicode_AsUTF8(infoPy);												//info
	PyObject *chapterlistPy = PyObject_GetAttrString(mainModule, "chapterlist");	//chapter-list
//	chapterlistbuf = PyUnicode_AsUTF8(chapterlistPy);	//used const char * instead
	
	//chapterlist parsing
	j = 1;
	
//	| method 1 - disabled for now |
	
//	char * chapterlist = PyUnicode_AsUTF8(chapterlistPy);
//	while (chapterlist) {
//		j = j + 1;
//		char * nextLine = strchr(chapterlist, '\n');
//		if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
//		chaptername[j] = chapterlist;
//		//printf("chapterlist=%s\n", chapterlist);
//		if (nextLine) *nextLine = '\n';  // then restore newline-char 
//		chapterlist = nextLine ? (nextLine+1) : NULL;
//		
//		//printf("j:%d %s\n", j, chaptername[j]);
//	}
	
//	| method 2 - longer than method 1, but works well enough |
	
	const char * chapterlist = PyUnicode_AsUTF8(chapterlistPy);
	while(chapterlist) {
		const char * nextLine = strchr(chapterlist, '\n');
		int chapterlistLen = nextLine ? (nextLine-chapterlist) : strlen(chapterlist);
		chaptername[j] = (char *) malloc(chapterlistLen+1);
		if (chaptername[j]) {
			memcpy(chaptername[j], chapterlist, chapterlistLen);
			chaptername[j][chapterlistLen] = '\0';  // NUL-terminate
//			printf("chaptername[j]=[%s]\n", chaptername[j]);	//debug purposes
			j = j + 1;
		}
		else printf("chapter parsing failed - please open an issue at github.com/Paranoid-Dev/ParanoidFFD\n");
		chapterlist = nextLine ? (nextLine+1) : NULL;
	}
	j = j - 1;
}

int next () {
	PyRun_SimpleString("try: next = chrome.find_element_by_xpath('//*[@id=\"content_wrapper_inner\"]/span/button[2]')\nexcept: next = chrome.find_element_by_xpath('//*[@id=\"content_wrapper_inner\"]/span/button')");
	PyRun_SimpleString("next.click()");
	PyRun_SimpleString("url = chrome.current_url");
	PyRun_SimpleString("chrome.quit()");
}

int print () {
	
//	cleaning up before exiting
	PyRun_SimpleString("chrome.quit()");
	Py_Finalize();
	
	int a = 1;
	int b = 1;
	printf("%s\n", title);
	printf("By %s\n", author);
	printf("Downloaded with ParanoidFFD, made with passion by Paranoid-Dev\n\n");
	printf("%s\n\n", info);
	printf("%s\n\n\n", summary);
	printf("Chapters\n\n");

	while (b <= j) {
		printf("%s\n", chaptername[b]);
		b = b + 1;
	}
	printf("\n\n\n");
	while (a <= j) {
		printf("| %s |\n\n", chaptername[a]);
		printf("%s\n\n", chapter[a]);
		a = a + 1;
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

			PyRun_SimpleString("yurl = input()");
			PyRun_SimpleString("zurl = yurl.replace('https://', '')");
			PyRun_SimpleString("furl = zurl.replace('http://', '')");
			PyRun_SimpleString("durl = furl.replace('www.', '')");
			PyRun_SimpleString("aurl = durl.replace('m.', '')");
			PyRun_SimpleString("burl = '/'.join(aurl.split('/', 3)[:3])");
			PyRun_SimpleString("url = f\"https://www.{burl}\"");
			
			description ();
			
			i = 0;
			
			while (1) {
				download ();
				if (i == j) {
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