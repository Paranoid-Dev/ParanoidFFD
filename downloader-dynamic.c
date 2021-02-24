#include <Python.h>	//	>= Python3.6
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const char* chapter[2048];
char* chaptername[2048];
const char *title, *author, *summary, *info, *filename, *totalchapters;
PyObject *mainModule;
int i, j;
int f = 1;
int fname = 1;

int description () {
	PyRun_SimpleString("options = uc.ChromeOptions()");
	PyRun_SimpleString("options.headless = True");
	PyRun_SimpleString("options.add_argument('--headless')");
	PyRun_SimpleString("chrome = uc.Chrome(options=options)");
	PyRun_SimpleString("chrome.get(url)");
	printf("Random delay before downloading to look unsuspicious\n");
	PyRun_SimpleString("sleep(random.uniform(7, 9))");	//delay 7~9 seconds randomly for first time page load
	printf("Downloading info...\n");
	PyRun_SimpleString("title = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/b').text");
	PyRun_SimpleString("author = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/a[1]').text");
	PyRun_SimpleString("summary = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/div').text");
	PyRun_SimpleString("try: info = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span[4]').text\nexcept: info = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span[3]').text");
	PyRun_SimpleString("try: chapterlist = chrome.find_element_by_xpath('//*[@id=\"chap_select\"]').text\nexcept: chapterlist = title");
	
	PyObject *titlePy = PyObject_GetAttrString(mainModule, "title");
	title = PyUnicode_AsUTF8(titlePy);												//title
	PyObject *authorPy = PyObject_GetAttrString(mainModule, "author");
	author = PyUnicode_AsUTF8(authorPy);											//author
	PyObject *summaryPy = PyObject_GetAttrString(mainModule, "summary");
	summary = PyUnicode_AsUTF8(summaryPy);											//summary
	PyObject *infoPy = PyObject_GetAttrString(mainModule, "info");
	info = PyUnicode_AsUTF8(infoPy);												//info
	PyObject *chapterlistPy = PyObject_GetAttrString(mainModule, "chapterlist");	//chapter-list
	totalchapters = PyUnicode_AsUTF8(chapterlistPy);	//for epub output
	printf("Fanfiction info downloaded\nParsing chapters...\n");
	//chapterlist parsing
	j = 1;

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
	printf("Parsed chapters\n");
	return 0;
}

/*
int print () {
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
	return 0;
}
*/

int help () {
	printf(" ________________________________________________________________________________________ \n");
	printf("                 Paranoid FanFiction Downloader v1.2.0.2  by Paranoid-Dev                 \n");
	printf("                       https://github.com/Paranoid-Dev/ParanoidFFD                        \n");
	printf(" ________________________________________________________________________________________ \n");
	printf("                                                                                          \n");
	printf(" Usage    : ParanoidFFD <options>                                                         \n");
	printf("                                                                                          \n");
	printf(" Options  :                                                                               \n");
	printf("   -u <fanfiction url> : download from <fanfiction url>                                   \n");
	printf("   -f <FORMAT>         : save fanfiction as <FORMAT>                                      \n");
	printf("                         currently supported format : txt, epub                           \n");
	printf("                         default : txt                                                    \n");
	printf("   -o <FILE_NAME>      : save fanfiction as <FILE_NAME>.<FORMAT>                          \n");
	printf("                         default : fanfiction_title.<FORMAT>                              \n");
	printf("   -C <Chrome Version> : set Chrome version as <Chrome Version>                           \n");
	printf("                         use if you're not on the latest stable Chrome build              \n");
	printf("                         example : -C 86 : for Chromium 86.0.4240.75                      \n");
	printf("   --version           : show ParanoidFFD version                                         \n");
	printf("   --check-update      : check for new updates                                            \n");
	printf("   -h , --help         : show this page                                                   \n");
	printf("                                                                                          \n");
	printf(" Examples :                                                                               \n");
	printf("   ParanoidFFD -u \"my/fan/fiction/url\"                                                    \n");	//since back slashes don't take up space,
	printf("   ParanoidFFD -f epub -o \"my fanfiction save\" -u \"my/fan/fiction/url\"                    \n");	//extra spaces are needed for uniform output.
	printf("   ParanoidFFD -C 86 --check-update                                                       \n");
	printf(" ________________________________________________________________________________________ \n");
	printf("                                                                                          \n");
	return 0;
}

int main (int argc, char *argv[]) {
	int p = 1;
	int down = 0;
	int chromever = 0;
	int argvurl;
	size_t l;
	char chromeversion[25];	//22+1+1+1
	if (argc == 1) {
		printf(" ________________________________________________________________________________________ \n");
		printf("                 Paranoid FanFiction Downloader v1.2.0.2  by Paranoid-Dev                 \n");
		printf("                       https://github.com/Paranoid-Dev/ParanoidFFD                        \n");
		printf(" ________________________________________________________________________________________ \n");
		printf(" \"ParanoidFFD --help\" to show help page                                                 \n");
		printf("                                                                                          \n");
	}
	else {
		while (p < argc) {
			if (strcmp(argv[p], "--version") == 0) {
				printf("ParanoidFFD 1.2.0.2\n");
			}
			else if (strcmp(argv[p], "--help") == 0) {
				help ();
			}
			else if (strcmp(argv[p], "-h") == 0) {
				help ();
			}
			else if (strcmp(argv[p], "-f") == 0) {
				p = p + 1;
				if (strcmp(argv[p], "txt") == 0) {
					f = 1;	//using format "txt"
				}
				else if (strcmp(argv[p], "epub") == 0) {
					f = 2;	//using format "epub"
				}
				else {
					printf("invalid format : %s\n", argv[p]);
					down = 0;
					break;
				}
			}
			else if (strcmp(argv[p], "-o") == 0) {
				p = p + 1;
				fname = 2;	//not using default filename
				filename = argv[p];
			}
			else if (strcmp(argv[p], "-u") == 0) {
				p = p + 1;
				argvurl = p;
				down = 1;
				l = strlen(argv[p]) + 12;
			}
			else if (strcmp(argv[p], "-C") == 0) {
				p = p + 1;
				chromever = 1;
				sprintf(chromeversion, "uc.TARGET_VERSION = %s",argv[p]);
			}
			else if (strcmp(argv[p], "--check-update") == 0) {
				remove("chromedriver.exe");
				remove("chromedriver");
				Py_Initialize();
				PyRun_SimpleString("import undetected_chromedriver as uc");
				if (chromever == 1) {
					PyRun_SimpleString(chromeversion);
				}
				mainModule = PyImport_AddModule("__main__");
				PyRun_SimpleString("options = uc.ChromeOptions()");
				PyRun_SimpleString("options.headless = True");
				PyRun_SimpleString("options.add_argument('--headless')");
				PyRun_SimpleString("chrome = uc.Chrome(options=options)");
				PyRun_SimpleString("chrome.get('https://raw.githubusercontent.com/Paranoid-Dev/ParanoidFFD/main/updates%20history/1.2.0.2-n')");
				PyRun_SimpleString("nextver = chrome.find_element_by_xpath('/html/body/pre').text");
				PyObject *nextverPy = PyObject_GetAttrString(mainModule, "nextver");
				const char * nextver = PyUnicode_AsUTF8(nextverPy);
				if (strcmp(nextver, "NA") == 0) {
					printf("ParanoidFFD is up to date! ParanoidFFD v1.2.0.2 by Paranoid-Dev\n");
				}
				else {
					printf("Current version : ParanoidFFD v1.2.0.2\nNew version : \n\n");
					PyRun_SimpleString("chrome.get(nextver)");
					PyRun_SimpleString("print(chrome.find_element_by_xpath('/html/body/pre').text)");
				}
				PyRun_SimpleString("chrome.quit()");
				Py_Finalize();
				remove("chromedriver.exe");
				remove("chromedriver");
			}
			else {
				printf("invalid argument : %s \n", argv[p]);
				down = 0;
				break;
			}
			p = p + 1;
		}
		
		if (down == 1) {
			printf("Downloading %s...\n",argv[argvurl]);
			
			remove("chromedriver.exe");
			remove("chromedriver");
			
			char buf[l];
			sprintf(buf, "yurl = \"%s\"",argv[argvurl]);
			
			Py_Initialize();
			//PyRun_SimpleString("import zipfile");
			PyRun_SimpleString("import undetected_chromedriver as uc");
			PyRun_SimpleString("from time import sleep");
			PyRun_SimpleString("import re");
			PyRun_SimpleString("import random");
			
			if (chromever == 1) {
				PyRun_SimpleString(chromeversion);
			}
			
			mainModule = PyImport_AddModule("__main__");
			
			printf("Loaded undetected chromedriver\n");
			
			PyRun_SimpleString(buf);
			PyRun_SimpleString("zurl = yurl.replace('https://', '')");
			PyRun_SimpleString("furl = zurl.replace('http://', '')");
			PyRun_SimpleString("durl = furl.replace('www.', '')");
			PyRun_SimpleString("aurl = durl.replace('m.', '')");
			PyRun_SimpleString("burl = '/'.join(aurl.split('/', 3)[:3])");
			PyRun_SimpleString("i = 1");
			PyRun_SimpleString("url = f\"https://www.{burl}/{i}\"");
		
			printf("Parsed fanfiction url\nFetching fanfiction info...\n");
			description ();
		
			i = 0;
			if (fname == 1) {	//using title as filename (default)
				filename = title;
			}
			
			while (1) {
				if (f == 1) {	//save to text
					if (i == j) {	//	download finished
						printf("Download finished\n");
						PyRun_SimpleString("chrome.quit()");
						Py_Finalize();
						remove("chromedriver.exe");
						remove("chromedriver");
						printf("Cleaning up.. - removed chromedriver\n");
					
						printf("writing to file...\n");
						int b = 1;
						size_t m;
						m = strlen(title) + strlen(author) + strlen(info) + strlen(summary) + 200; //+101+88+1(??)
						
						while (b <= j) {
							m = m + strlen(chaptername[b]) + 2;
							b = b + 1;
						}
						b = 1;
						while (b <= j) {
							m = m + strlen(chaptername[b]) + strlen(chapter[b]) + 12;
							b = b + 1;
						}
						b = 1;
						
						char buf1[m];
						char buf2[m];
						sprintf(buf2, "%s\nBy %s\nDownloaded with ParanoidFFD, made with passion by Paranoid-Dev\n\n%s\n\n%s\n\n\nChapters\n\n",title,author,info,summary);
						
						while (b <= j) {
							sprintf(buf1, "%s%s\n",buf2,chaptername[b]);
							sprintf(buf2, "%s",buf1);
							b = b + 1;
						}
						b = 1;
						sprintf(buf1, "%s\n\n\n",buf2);
						while (b <= j) {
							sprintf(buf2, "%s| %s |\n\n",buf1,chaptername[b]);
							sprintf(buf1, "%s%s\n\n",buf2,chapter[b]);
							b = b + 1;
						}

						printf("saving to \"%s.txt\"\n",filename);
						FILE *fp;
						char output[strlen(filename)+5];
						sprintf(output, "%s.txt",filename);
						fp = fopen(output, "w+");
						fprintf(fp, "%s",buf1);
						fclose(fp);
						printf("Finished!\n");
						break;
					}
					//download chapters
					i = i + 1;
					printf("Fetching chapter %d...\n",i);
					//	chrome initialzation unneeded?
					PyRun_SimpleString("options = uc.ChromeOptions()");
					PyRun_SimpleString("options.headless = True");
					PyRun_SimpleString("options.add_argument('--headless')");
					PyRun_SimpleString("chrome = uc.Chrome(options=options)");
					
					PyRun_SimpleString("chrome.get(url)");
					printf("Random delay before downloading to look unsuspicious\n");
					PyRun_SimpleString("sleep(random.uniform(1, 2))");	//delay 1~2 seconds randomly
					printf("Downloading chapter %d...\n",i);
					PyRun_SimpleString("chapter = chrome.find_element_by_xpath('//*[@id=\"storytext\"]').text");

					PyObject *chapterPy = PyObject_GetAttrString(mainModule, "chapter");
					chapter[i] = PyUnicode_AsUTF8(chapterPy);
					printf("Chapter %d downloaded\n",i);
				}
				else if (f == 2) {	//save to epub
					if (i == j) {	//download finished
						printf("Download finished\n");
						PyRun_SimpleString("chrome.quit()");
						remove("chromedriver.exe");
						remove("chromedriver");
						printf("Cleaning up.. - removed chromedriver\n");
						printf("writing to file...\n");
						
						//write to file
						//To people (including me) trying to edit this below. I am sorry. This was the best I could do without using more dependencies. I am so sorry.
						unsigned char pre_ParanoidFFD_Template_epub[781] = {0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x0c, 0x54, 0x52, 0x6f, 0x61, 0xab, 0x2c, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x6d, 0x69, 0x6d, 0x65, 0x74, 0x79, 0x70, 0x65, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x65, 0x70, 0x75, 0x62, 0x2b, 0x7a, 0x69, 0x70, 0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x58, 0x54, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x1c, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x55, 0x54, 0x09, 0x00, 0x03, 0xbf, 0x6d, 0x30, 0x60, 0x6f, 0xd9, 0x30, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x03, 0x04, 0x14, 0x00, 0x02, 0x00, 0x08, 0x00, 0xce, 0x95, 0x54, 0x52, 0xaa, 0x1e, 0x0e, 0x69, 0x95, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x16, 0x00, 0x1c, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x69, 0x6e, 0x65, 0x72, 0x2e, 0x78, 0x6d, 0x6c, 0x55, 0x54, 0x09, 0x00, 0x03, 0x74, 0xda, 0x30, 0x60, 0x75, 0xda, 0x30, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x55, 0x8e, 0x4d, 0x0e, 0xc2, 0x20, 0x10, 0x85, 0xf7, 0x3d, 0x05, 0x61, 0x6b, 0x5a, 0x74, 0x4b, 0xa0, 0x3d, 0xcb, 0x48, 0xa7, 0x4a, 0x84, 0x19, 0x02, 0xd4, 0xe8, 0xed, 0xa5, 0x5d, 0x34, 0x75, 0xf9, 0xf2, 0x7e, 0xbe, 0x67, 0xa6, 0x4f, 0x0c, 0xe2, 0x8d, 0xb9, 0x78, 0x26, 0x2b, 0x6f, 0xc3, 0x55, 0x4e, 0x63, 0x67, 0x1c, 0x53, 0x05, 0x4f, 0x98, 0xff, 0x2d, 0xd1, 0xc2, 0x54, 0xac, 0x5c, 0x33, 0x69, 0x86, 0xe2, 0x8b, 0x26, 0x88, 0x58, 0x74, 0x75, 0x9a, 0x13, 0xd2, 0xcc, 0x6e, 0x8d, 0x48, 0x55, 0xef, 0x31, 0x7d, 0x8c, 0xc8, 0xb1, 0x13, 0xc2, 0x64, 0xe6, 0xba, 0xf8, 0x80, 0x65, 0x53, 0x27, 0x2d, 0x96, 0x35, 0x84, 0x3e, 0x41, 0x7d, 0x5a, 0xb9, 0x55, 0xda, 0xc0, 0xc0, 0x69, 0x91, 0x22, 0xe2, 0xec, 0xa1, 0xaf, 0xdf, 0x84, 0x56, 0x42, 0x4a, 0xc1, 0x3b, 0xa8, 0xed, 0x8a, 0x62, 0xbc, 0xa7, 0xd2, 0x0a, 0xee, 0x05, 0x0f, 0xbc, 0x34, 0x96, 0x54, 0x3b, 0x40, 0x9d, 0x08, 0x46, 0x1d, 0xf4, 0xf1, 0x07, 0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x02, 0x00, 0x00, 0x00, 0xcb, 0x58, 0x54, 0x52, 0x63, 0x43, 0x21, 0xf0, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x1c, 0x00, 0x74, 0x65, 0x6d, 0x70, 0x6c, 0x61, 0x74, 0x65, 0x2e, 0x63, 0x73, 0x73, 0x55, 0x54, 0x09, 0x00, 0x03, 0x9e, 0x6e, 0x30, 0x60, 0xa9, 0xd9, 0x30, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x68, 0x31, 0x20, 0x7b, 0x0a, 0x20, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2d, 0x61, 0x6c, 0x69, 0x67, 0x6e, 0x3a, 0x20, 0x63, 0x65, 0x6e, 0x74, 0x65, 0x72, 0x3b, 0x0a, 0x7d, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x0c, 0x54, 0x52, 0x6f, 0x61, 0xab, 0x2c, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x81, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x69, 0x6d, 0x65, 0x74, 0x79, 0x70, 0x65, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x58, 0x54, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xed, 0x41, 0x3a, 0x00, 0x00, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x55, 0x54, 0x05, 0x00, 0x03, 0xbf, 0x6d, 0x30, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x14, 0x00, 0x02, 0x00, 0x08, 0x00, 0xce, 0x95, 0x54, 0x52, 0xaa, 0x1e, 0x0e, 0x69, 0x95, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x16, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xa4, 0x81, 0x7d, 0x00, 0x00, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x69, 0x6e, 0x65, 0x72, 0x2e, 0x78, 0x6d, 0x6c, 0x55, 0x54, 0x05, 0x00, 0x03, 0x74, 0xda, 0x30, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x0a, 0x00, 0x02, 0x00, 0x00, 0x00, 0xcb, 0x58, 0x54, 0x52, 0x63, 0x43, 0x21, 0xf0, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xa4, 0x81, 0x62, 0x01, 0x00, 0x00, 0x74, 0x65, 0x6d, 0x70, 0x6c, 0x61, 0x74, 0x65, 0x2e, 0x63, 0x73, 0x73, 0x55, 0x54, 0x05, 0x00, 0x03, 0x9e, 0x6e, 0x30, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x33, 0x01, 0x00, 0x00, 0xc4, 0x01, 0x00, 0x00, 0x00, 0x00};
						char output[strlen(filename)+6];
						sprintf(output, "%s.epub",filename);
						FILE *epub_out;
						epub_out = fopen(output,"wb");
						fwrite(pre_ParanoidFFD_Template_epub,781,1,epub_out); 
						fclose(epub_out);
						
						//content.opf
						char contentopf[strlen(title) + strlen(argv[argvurl]) + 130*j + 855];
						char contentopf_manifest_chapters[90*j];
						char bufepub[90*j];
						char contentopf_spine_chapters[35*j];
						char bufepub2[35*j];
						int b = 1;
						sprintf(contentopf_manifest_chapters, "    <item id=\"chapter_%d\" href=\"chapter_%d.xhtml\" media-type=\"application/xhtml+xml\" />\n",b,b);
						sprintf(contentopf_spine_chapters, "    <itemref idref=\"chapter_%d\" />\n",b);
						b = b + 1;
						while (b <= j) {
							sprintf(bufepub, "%s",contentopf_manifest_chapters);
							sprintf(bufepub2, "%s",contentopf_spine_chapters);
							sprintf(contentopf_manifest_chapters, "%s    <item id=\"chapter_%d\" href=\"chapter_%d.xhtml\" media-type=\"application/xhtml+xml\" />\n",bufepub,b,b);
							sprintf(contentopf_spine_chapters, "%s    <itemref idref=\"chapter_%d\" />\n",bufepub2,b);
							b = b + 1;
						}
						time_t now;	//getting date-time
						time(&now);
						struct tm* now_tm;
						now_tm = localtime(&now);
						char datetime[22];
						strftime (datetime, 22, "%FT%TZ", now_tm);
						
						sprintf(contentopf, "zf.writestr(\"content.opf\", \"\"\"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<package xmlns=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" unique-identifier=\"db-id\" version=\"3.0\">\n\n<metadata>\n    <dc:title id=\"t1\">%s</dc:title>\n    <dc:identifier id=\"db-id\">%s-ParanoidFFD</dc:identifier>\n    <meta property=\"dcterms:modified\">%s</meta>\n    <dc:language>en</dc:language>\n</metadata>\n\n<manifest>\n    <item id=\"toc\" properties=\"nav\" href=\"toc.xhtml\" media-type=\"application/xhtml+xml\" />\n    <item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" />\n    <item id=\"template_css\" href=\"template.css\" media-type=\"text/css\" />\n    <item id=\"cover\" href=\"cover.xhtml\" media-type=\"application/xhtml+xml\" />\n%s</manifest>\n\n<spine toc=\"ncx\">\n    <itemref idref=\"cover\" />\n%s</spine>\n\n</package>\"\"\")",title,argv[argvurl],datetime,contentopf_manifest_chapters,contentopf_spine_chapters);	//end line
						
						//toc.ncx
						char tocncx[strlen(title) + strlen(argv[argvurl]) + strlen(totalchapters) + 165*j + 490];
						char tocncx_navmap[strlen(totalchapters) + 165*j];
						char tocncxbuf[strlen(totalchapters) + 165*j];
						b = 1;
						sprintf(tocncx_navmap, "    <navPoint id=\"chapter_%d\" playOrder=\"%d\">\n        <navLabel><text>%s</text></navLabel>\n        <content src=\"chapter_%d.xhtml\" />\n    </navPoint>\n",b,b+1,chaptername[b],b);
						b = b + 1;
						while (b <= j) {
							sprintf(tocncxbuf, "%s",tocncx_navmap);
							sprintf(tocncx_navmap, "%s    <navPoint id=\"chapter_%d\" playOrder=\"%d\">\n        <navLabel><text>%s</text></navLabel>\n        <content src=\"chapter_%d.xhtml\" />\n    </navPoint>\n",tocncxbuf,b,b+1,chaptername[b],b);
							b = b + 1;
						}
						
						sprintf(tocncx, "zf.writestr(\"toc.ncx\", \"\"\"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<ncx version=\"2005-1\" xml:lang=\"en\" xmlns=\"http://www.daisy.org/z3986/2005/ncx/\">\n\n<head>\n    <meta name=\"dtb:uid\" content=\"%s-ParanoidFFD\"/>\n    <meta name=\"dtb:depth\" content=\"1\"/>\n</head>\n\n<docTitle>\n    <text></text>\n</docTitle>\n\n<navMap>\n    <navPoint id=\"cover\" playOrder=\"1\">\n        <navLabel><text>%s</text></navLabel>\n        <content src=\"cover.xhtml\" />\n    </navPoint>\n%s</navMap>\n\n</ncx>\"\"\")",argv[argvurl],title,tocncx_navmap);	//end line
						
						//toc.xhtml
						char tocxhtml[strlen(title) + strlen(totalchapters) + 60*j + 510];
						char toc_contents[strlen(totalchapters) + 60*j];
						char tocbuf[strlen(totalchapters) + 60*j];
						b = 1;
						sprintf(toc_contents, "            <li><a href=\"chapter_%d.xhtml\">%s</a></li>\n",b,chaptername[b]);
						b = b + 1;
						while (b <= j) {
							sprintf(tocbuf, "%s",toc_contents);
							sprintf(toc_contents, "%s            <li><a href=\"chapter_%d.xhtml\">%s</a></li>\n",tocbuf,b,chaptername[b]);
							b = b + 1;
						}
						
						sprintf(tocxhtml, "zf.writestr(\"toc.xhtml\", \"\"\"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">\n<head>\n<title>toc.xhtml</title>\n<link href=\"template.css\" rel=\"stylesheet\" type=\"text/css\" />\n</head>\n\n<body>\n\n    <nav id=\"toc\" epub:type=\"toc\">\n        <h1 class=\"frontmatter\">Table of Contents</h1>\n        <ol class=\"contents\">\n            <li><a href=\"cover.xhtml\">%s</a></li>\n%s        </ol>\n\n    </nav>\n\n</body>\n\n</html>\"\"\")",title,toc_contents);
						
						//cover.xhtml
						char cover[strlen(title)*j + strlen(author) + strlen(info) + strlen(summary) + 465];
						sprintf(cover, "zf.writestr(\"cover.xhtml\", \"\"\"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">\n<head>\n<title>%s</title>\n<link href=\"template.css\" rel=\"stylesheet\" type=\"text/css\" />\n</head>\n\n<body>\n\n    <h1>%s</h1>\n    <h3>By %s</h3>\n    <p><small>Downloaded with ParanoidFFD, made with passion by Paranoid-Dev</small></p>\n    <br />\n    <p>%s</p>\n    <br />\n    <p>%s</p>\n\n</body>\n</html>\"\"\")",title,title,author,info,summary);
						
						//write metadata to epub zip
						//PyRun_SimpleString("zf.writestr(\"content.opf\", \"\"\"content.opf file string\"\"\")");
						
						char writeoutput[strlen(output) + 75];
						sprintf(writeoutput, "zf = zipfile.ZipFile(\"%s\", mode=\"a\", compression=zipfile.ZIP_DEFLATED)",output);
						
						PyRun_SimpleString("import zipfile");
						PyRun_SimpleString(writeoutput);
						PyRun_SimpleString(contentopf);
						PyRun_SimpleString(tocncx);
						PyRun_SimpleString(tocxhtml);
						PyRun_SimpleString(cover);
						
						//chapters
						int chapterlen = strlen(chapter[1]) + strlen(chaptername[1])*2;
						for(b = 2; b <= j; b++) {
							if(chapterlen < strlen(chapter[b]) + strlen(chaptername[b])*2) {
								chapterlen = strlen(chapter[b]) + strlen(chaptername[b])*2;
							}
						}
						char epubchapter[chapterlen + 275];
						b = 1;
						while (b <= j) {
							sprintf(epubchapter, "zf.writestr(\"chapter_%d.xhtml\", \"\"\"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">\n<head>\n<title>%s</title>\n<link href=\"template.css\" rel=\"stylesheet\" type=\"text/css\" />\n</head>\n\n<body>\n\n    <h1>%s</h1>\n    <br />\n    %s\n</body>\n</html>\"\"\")",b,chaptername[b],chaptername[b],chapter[b]);
							PyRun_SimpleString(epubchapter);
							b = b + 1;
						}
						
						//finish
						PyRun_SimpleString("zf.close()");
						Py_Finalize();
						
						printf("Finished!\n");
						break;
					}
					//download chapters
					i = i + 1;
					printf("Fetching chapter %d...\n",i);
					//	chrome initialzation unneeded?
					PyRun_SimpleString("options = uc.ChromeOptions()");
					PyRun_SimpleString("options.headless = True");
					PyRun_SimpleString("options.add_argument('--headless')");
					PyRun_SimpleString("chrome = uc.Chrome(options=options)");
					
					PyRun_SimpleString("chrome.get(url)");
					printf("Random delay before downloading to look unsuspicious\n");
					PyRun_SimpleString("sleep(random.uniform(1, 2))");	//delay 1~2 seconds randomly
					printf("Downloading chapter %d...\n",i);
					PyRun_SimpleString("chapter = chrome.find_element_by_xpath('//*[@id=\"storytext\"]').get_attribute(\"innerHTML\")");
					//Parsing html to make it work as xhtml, and removing embeded ads
					PyRun_SimpleString("chapter = re.sub('<div.*?</div>','',chapter, flags=re.DOTALL)");
					//should find more elegant solution
					PyRun_SimpleString("chapter = re.sub('<p .*?>','<p>',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<hr.*?>','<hr />',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<br.*?>','<br />',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<area.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<base.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<col.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<embed.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<img.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<input.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<link.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<meta.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<param.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<source.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<track.*?>','',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub('<wbr.*?>','',chapter, flags=re.DOTALL)");
					
					PyObject *chapterPy = PyObject_GetAttrString(mainModule, "chapter");
					chapter[i] = PyUnicode_AsUTF8(chapterPy);
					printf("Chapter %d downloaded\n",i);
				}
				PyRun_SimpleString("i += 1");
				PyRun_SimpleString("url = f\"https://www.{burl}/{i}\"");	
			}
		}
	}
	return 0;
}