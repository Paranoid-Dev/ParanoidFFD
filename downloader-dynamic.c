#include <Python.h>	//	>= Python3.6
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const char* chapter[2048];
char* chaptername[2048];
const char *title, *author, *summary, *info, *filename, *totalchapters;
PyObject *mainModule;
time_t updatedunixtimestamp;
time_t publishedunixtimestamp;
int i, j;
int f = 1;
int fname = 1;
char updated[32];
char published[32];
char datetime[22];
char mark[32];

void description () {
	PyRun_SimpleString("options = uc.ChromeOptions()");
	PyRun_SimpleString("options.headless = True");
	PyRun_SimpleString("options.add_argument('--headless')");
	PyRun_SimpleString("chrome = uc.Chrome(options=options)");
	PyRun_SimpleString("chrome.get(url)");
//	printf("Random delay before downloading to look unsuspicious\n");
//	PyRun_SimpleString("sleep(random.uniform(7, 9))");	//delay 7~9 seconds randomly for first time page load
	printf("Downloading info...\n");
	PyRun_SimpleString("title = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/b').text");
	PyRun_SimpleString("author = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/a[1]').text");
	PyRun_SimpleString("summary = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/div').text");
	PyRun_SimpleString("try: info = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span[4]').text\nexcept: info = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span[3]').text");
	PyRun_SimpleString("try: chapterlist = chrome.find_element_by_xpath('//*[@id=\"chap_select\"]').text\nexcept: chapterlist = title");
	PyRun_SimpleString("updated = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span/span[1]').get_attribute(\"outerHTML\")");
	PyRun_SimpleString("try: published = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span/span[2]').get_attribute(\"outerHTML\")\nexcept: published = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span/span[1]').get_attribute(\"outerHTML\")");
	
	PyObject *titlePy = PyObject_GetAttrString(mainModule, "title");
	title = PyUnicode_AsUTF8(titlePy);												//title
	PyObject *authorPy = PyObject_GetAttrString(mainModule, "author");
	author = PyUnicode_AsUTF8(authorPy);											//author
	PyObject *summaryPy = PyObject_GetAttrString(mainModule, "summary");
	summary = PyUnicode_AsUTF8(summaryPy);											//summary
	
	PyRun_SimpleString("updated = re.sub(r'.*?\"(.*?)\".*',r'\\1',updated, flags=re.DOTALL)");
	PyRun_SimpleString("published = re.sub(r'.*?\"(.*?)\".*',r'\\1',published, flags=re.DOTALL)");
	
	PyObject *updatedPy = PyObject_GetAttrString(mainModule, "updated");
	updatedunixtimestamp = strtol(PyUnicode_AsUTF8(updatedPy), NULL, 10);	
	struct tm *updatedtimestamp = gmtime(&updatedunixtimestamp);
	strftime (updated, 32, "on %F, at %T", updatedtimestamp);
	PyObject *publishedPy = PyObject_GetAttrString(mainModule, "published");
	publishedunixtimestamp = strtol(PyUnicode_AsUTF8(publishedPy), NULL, 10);	
	struct tm *publishedtimestamp = gmtime(&publishedunixtimestamp);
	strftime (published, 32, "on %F, at %T", publishedtimestamp);
	
	
	
	
//	PyRun_SimpleString("lang = info");	//regex this for language
	PyRun_SimpleString("info = re.sub(r'(Reviews|Favs|Follows|Published|Updated).*?- ',r'',info, flags=re.DOTALL)");
	PyRun_SimpleString("info = re.sub(r'- ',r'\\n',info, flags=re.DOTALL)");
	PyRun_SimpleString("info = re.sub(r'Rated: ',r'',info, flags=re.DOTALL)");
	PyRun_SimpleString("info = re.sub(r'id:.*$',r'',info, flags=re.DOTALL)");
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
}

void help () {
	printf(" ________________________________________________________________________________________ \n");
	printf("                 Paranoid FanFiction Downloader v1.2.1.2  by Paranoid-Dev                 \n");
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
}

int main (int argc, char *argv[]) {
	int p = 1;
	int down = 0;
	int chromever = 0;
	int argvurl;
	size_t l;
	char chromeversion[25];
	if (argc == 1) {
		printf(" ________________________________________________________________________________________ \n");
		printf("                 Paranoid FanFiction Downloader v1.2.1.2  by Paranoid-Dev                 \n");
		printf("                       https://github.com/Paranoid-Dev/ParanoidFFD                        \n");
		printf(" ________________________________________________________________________________________ \n");
		printf(" \"ParanoidFFD --help\" to show help page                                                 \n");
		printf("                                                                                          \n");
	}
	else {
		while (p < argc) {
			if (strcmp(argv[p], "--version") == 0) {
				printf("ParanoidFFD 1.2.1.2\n");
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
				PyRun_SimpleString("chrome.get('https://raw.githubusercontent.com/Paranoid-Dev/ParanoidFFD/main/updates%20history/1.2.1.2-n')");
				PyRun_SimpleString("nextver = chrome.find_element_by_xpath('/html/body/pre').text");
				PyObject *nextverPy = PyObject_GetAttrString(mainModule, "nextver");
				const char * nextver = PyUnicode_AsUTF8(nextverPy);
				if (strcmp(nextver, "NA") == 0) {
					printf("ParanoidFFD is up to date! ParanoidFFD v1.2.1.2 by Paranoid-Dev\n");
				}
				else {
					printf("ParanoidFFD isn't up to date. Fetching updates info..\nNew version : \n\n");
					PyRun_SimpleString("chrome.get(nextver)");
					PyRun_SimpleString("print(chrome.find_element_by_xpath('/html/body/pre').text)");
					printf("\nCurrent version : ParanoidFFD v1.2.1.2\n");
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
			
			time_t now;	//getting date-time
			time(&now);
			struct tm* now_tm;
			now_tm = gmtime(&now);
			strftime (datetime, 22, "%FT%TZ", now_tm);
			strftime (mark, 32, "on %F, at %T", now_tm);
			
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
						m = strlen(title) + strlen(author) + strlen(info) + strlen(summary) + strlen(mark) + 215;
						
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
						sprintf(buf2, "%s\nBy %s\n\n%sPublished %s\nUpdated %s\nDownloaded with ParanoidFFD, %s\n\n%s\n\n\nChapters\n\n",title,author,info,published,updated,mark,summary);
						
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
						unsigned char pre_ParanoidFFD_Template_epub[1275] = {0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x0c, 0x54, 0x52, 0x6f, 0x61, 0xab, 0x2c, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x6d, 0x69, 0x6d, 0x65, 0x74, 0x79, 0x70, 0x65, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x65, 0x70, 0x75, 0x62, 0x2b, 0x7a, 0x69, 0x70, 0x50, 0x4b, 0x03, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xeb, 0x5d, 0x5b, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x1c, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x55, 0x54, 0x09, 0x00, 0x03, 0xb9, 0xb2, 0x39, 0x60, 0xba, 0xb2, 0x39, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x03, 0x04, 0x14, 0x00, 0x02, 0x00, 0x08, 0x00, 0x29, 0x62, 0x5b, 0x52, 0xef, 0x71, 0x90, 0xc7, 0xa5, 0x00, 0x00, 0x00, 0x6f, 0x01, 0x00, 0x00, 0x2d, 0x00, 0x1c, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x63, 0x6f, 0x6d, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e, 0x69, 0x62, 0x6f, 0x6f, 0x6b, 0x73, 0x2e, 0x64, 0x69, 0x73, 0x70, 0x6c, 0x61, 0x79, 0x2d, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x2e, 0x78, 0x6d, 0x6c, 0x55, 0x54, 0x09, 0x00, 0x03, 0xbe, 0xb9, 0x39, 0x60, 0xda, 0xb2, 0x39, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x85, 0x90, 0x31, 0x0e, 0xc2, 0x30, 0x0c, 0x45, 0xf7, 0x9e, 0x22, 0xca, 0x88, 0x14, 0x0a, 0x1b, 0x43, 0x5a, 0x36, 0x4e, 0x00, 0x33, 0x8a, 0x12, 0x17, 0x59, 0xb4, 0x76, 0x94, 0x98, 0x0a, 0x6e, 0x4f, 0x4b, 0x61, 0xa2, 0x05, 0x8f, 0xdf, 0xef, 0xdb, 0xfe, 0xb6, 0xfb, 0x7b, 0xd7, 0xaa, 0x1e, 0x52, 0x46, 0xa6, 0x4a, 0x6f, 0xd7, 0x1b, 0xad, 0x80, 0x3c, 0x07, 0xa4, 0x4b, 0xa5, 0x4f, 0xc7, 0x83, 0xd9, 0xe9, 0x7d, 0x5d, 0xd8, 0x80, 0x39, 0xb6, 0xee, 0x71, 0xe6, 0x28, 0x03, 0x98, 0xeb, 0x42, 0x0d, 0x65, 0x07, 0x49, 0x1a, 0x4e, 0x9d, 0x22, 0xd7, 0x41, 0xa5, 0x57, 0x7a, 0xd2, 0x5f, 0xbd, 0x89, 0x7c, 0x77, 0x72, 0x04, 0x8f, 0x0d, 0x42, 0x30, 0x0d, 0x93, 0x64, 0x5d, 0x4b, 0xba, 0x81, 0x2d, 0x27, 0x66, 0xc9, 0xc4, 0x09, 0x81, 0xc4, 0x8d, 0x8a, 0x69, 0xd9, 0x5f, 0x75, 0x4d, 0x4c, 0x7f, 0x5d, 0x48, 0x02, 0xc9, 0x79, 0xc1, 0x1e, 0xe6, 0xd6, 0xd8, 0xf2, 0x73, 0xf4, 0x7c, 0x06, 0x8c, 0x2e, 0x2c, 0xc6, 0xe0, 0x08, 0x64, 0x84, 0x4d, 0x8e, 0x09, 0x46, 0xec, 0xf7, 0x78, 0x5b, 0x7e, 0x7d, 0xed, 0x09, 0x50, 0x4b, 0x03, 0x04, 0x14, 0x00, 0x02, 0x00, 0x08, 0x00, 0xce, 0x95, 0x54, 0x52, 0xaa, 0x1e, 0x0e, 0x69, 0x95, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x16, 0x00, 0x1c, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x69, 0x6e, 0x65, 0x72, 0x2e, 0x78, 0x6d, 0x6c, 0x55, 0x54, 0x09, 0x00, 0x03, 0x74, 0xda, 0x30, 0x60, 0x22, 0xb9, 0x38, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x55, 0x8e, 0x4d, 0x0e, 0xc2, 0x20, 0x10, 0x85, 0xf7, 0x3d, 0x05, 0x61, 0x6b, 0x5a, 0x74, 0x4b, 0xa0, 0x3d, 0xcb, 0x48, 0xa7, 0x4a, 0x84, 0x19, 0x02, 0xd4, 0xe8, 0xed, 0xa5, 0x5d, 0x34, 0x75, 0xf9, 0xf2, 0x7e, 0xbe, 0x67, 0xa6, 0x4f, 0x0c, 0xe2, 0x8d, 0xb9, 0x78, 0x26, 0x2b, 0x6f, 0xc3, 0x55, 0x4e, 0x63, 0x67, 0x1c, 0x53, 0x05, 0x4f, 0x98, 0xff, 0x2d, 0xd1, 0xc2, 0x54, 0xac, 0x5c, 0x33, 0x69, 0x86, 0xe2, 0x8b, 0x26, 0x88, 0x58, 0x74, 0x75, 0x9a, 0x13, 0xd2, 0xcc, 0x6e, 0x8d, 0x48, 0x55, 0xef, 0x31, 0x7d, 0x8c, 0xc8, 0xb1, 0x13, 0xc2, 0x64, 0xe6, 0xba, 0xf8, 0x80, 0x65, 0x53, 0x27, 0x2d, 0x96, 0x35, 0x84, 0x3e, 0x41, 0x7d, 0x5a, 0xb9, 0x55, 0xda, 0xc0, 0xc0, 0x69, 0x91, 0x22, 0xe2, 0xec, 0xa1, 0xaf, 0xdf, 0x84, 0x56, 0x42, 0x4a, 0xc1, 0x3b, 0xa8, 0xed, 0x8a, 0x62, 0xbc, 0xa7, 0xd2, 0x0a, 0xee, 0x05, 0x0f, 0xbc, 0x34, 0x96, 0x54, 0x3b, 0x40, 0x9d, 0x08, 0x46, 0x1d, 0xf4, 0xf1, 0x07, 0x50, 0x4b, 0x03, 0x04, 0x14, 0x00, 0x02, 0x00, 0x08, 0x00, 0x9c, 0x0d, 0x5b, 0x52, 0x00, 0xe1, 0x2b, 0xcd, 0x8b, 0x00, 0x00, 0x00, 0x12, 0x01, 0x00, 0x00, 0x0c, 0x00, 0x1c, 0x00, 0x74, 0x65, 0x6d, 0x70, 0x6c, 0x61, 0x74, 0x65, 0x2e, 0x63, 0x73, 0x73, 0x55, 0x54, 0x09, 0x00, 0x03, 0x87, 0x25, 0x39, 0x60, 0xf8, 0x25, 0x39, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x6d, 0xce, 0xcf, 0x0e, 0x83, 0x20, 0x0c, 0x06, 0xf0, 0xbb, 0x4f, 0x41, 0xb2, 0xec, 0xc8, 0x61, 0x1e, 0xf1, 0x69, 0x0a, 0x56, 0x64, 0x62, 0x21, 0x50, 0xb3, 0x11, 0xb3, 0x77, 0x9f, 0xba, 0x3f, 0xc9, 0xc2, 0x7a, 0xeb, 0xf7, 0x6b, 0x93, 0x4f, 0x87, 0xbe, 0x88, 0xb5, 0x11, 0x42, 0x83, 0x99, 0x6c, 0x0a, 0x0b, 0xf5, 0xd2, 0x04, 0x1f, 0x92, 0x12, 0xa7, 0xe1, 0x98, 0x6e, 0xc3, 0x19, 0x92, 0x75, 0xa4, 0x44, 0x7b, 0xde, 0x37, 0x89, 0x71, 0xd1, 0x72, 0x2c, 0x71, 0x44, 0xca, 0x4a, 0x50, 0x20, 0x3c, 0xe2, 0x1b, 0xea, 0xc9, 0xf1, 0x1f, 0x98, 0x73, 0x1d, 0x56, 0x01, 0xf4, 0x41, 0xe3, 0xfb, 0x0e, 0x18, 0x3f, 0xf0, 0x68, 0x9a, 0xf1, 0x72, 0x34, 0x64, 0xbc, 0xb3, 0x04, 0xef, 0xec, 0x56, 0xc4, 0x20, 0x31, 0xa6, 0x97, 0xb6, 0x95, 0x7a, 0x1c, 0x78, 0xb7, 0x58, 0xc9, 0x75, 0xc9, 0xec, 0x86, 0xd2, 0xfd, 0xc4, 0xd2, 0x43, 0xe6, 0xef, 0xd7, 0x13, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x0c, 0x54, 0x52, 0x6f, 0x61, 0xab, 0x2c, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x81, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x69, 0x6d, 0x65, 0x74, 0x79, 0x70, 0x65, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xeb, 0x5d, 0x5b, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0xed, 0x41, 0x3a, 0x00, 0x00, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x55, 0x54, 0x05, 0x00, 0x03, 0xb9, 0xb2, 0x39, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x14, 0x00, 0x02, 0x00, 0x08, 0x00, 0x29, 0x62, 0x5b, 0x52, 0xef, 0x71, 0x90, 0xc7, 0xa5, 0x00, 0x00, 0x00, 0x6f, 0x01, 0x00, 0x00, 0x2d, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xa4, 0x81, 0x7d, 0x00, 0x00, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x63, 0x6f, 0x6d, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e, 0x69, 0x62, 0x6f, 0x6f, 0x6b, 0x73, 0x2e, 0x64, 0x69, 0x73, 0x70, 0x6c, 0x61, 0x79, 0x2d, 0x6f, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x2e, 0x78, 0x6d, 0x6c, 0x55, 0x54, 0x05, 0x00, 0x03, 0xbe, 0xb9, 0x39, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x14, 0x00, 0x02, 0x00, 0x08, 0x00, 0xce, 0x95, 0x54, 0x52, 0xaa, 0x1e, 0x0e, 0x69, 0x95, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00, 0x16, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xa4, 0x81, 0x89, 0x01, 0x00, 0x00, 0x4d, 0x45, 0x54, 0x41, 0x2d, 0x49, 0x4e, 0x46, 0x2f, 0x63, 0x6f, 0x6e, 0x74, 0x61, 0x69, 0x6e, 0x65, 0x72, 0x2e, 0x78, 0x6d, 0x6c, 0x55, 0x54, 0x05, 0x00, 0x03, 0x74, 0xda, 0x30, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x01, 0x02, 0x1e, 0x03, 0x14, 0x00, 0x02, 0x00, 0x08, 0x00, 0x9c, 0x0d, 0x5b, 0x52, 0x00, 0xe1, 0x2b, 0xcd, 0x8b, 0x00, 0x00, 0x00, 0x12, 0x01, 0x00, 0x00, 0x0c, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xa4, 0x81, 0x6e, 0x02, 0x00, 0x00, 0x74, 0x65, 0x6d, 0x70, 0x6c, 0x61, 0x74, 0x65, 0x2e, 0x63, 0x73, 0x73, 0x55, 0x54, 0x05, 0x00, 0x03, 0x87, 0x25, 0x39, 0x60, 0x75, 0x78, 0x0b, 0x00, 0x01, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x04, 0xe8, 0x03, 0x00, 0x00, 0x50, 0x4b, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0xa6, 0x01, 0x00, 0x00, 0x3f, 0x03, 0x00, 0x00, 0x00, 0x00};
						char output[strlen(filename)+6];
						sprintf(output, "%s.epub",filename);
						FILE *epub_out;
						epub_out = fopen(output,"wb");
						fwrite(pre_ParanoidFFD_Template_epub,1275,1,epub_out); 
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
						
						sprintf(tocncx, "zf.writestr(\"toc.ncx\", \"\"\"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<ncx version=\"2005-1\" xml:lang=\"en\" xmlns=\"http://www.daisy.org/z3986/2005/ncx/\">\n\n<head>\n    <meta name=\"dtb:uid\" content=\"%s-ParanoidFFD\"/>\n    <meta name=\"dtb:depth\" content=\"1\"/>\n</head>\n\n<docTitle>\n    <text>toc</text>\n</docTitle>\n\n<navMap>\n    <navPoint id=\"cover\" playOrder=\"1\">\n        <navLabel><text>%s</text></navLabel>\n        <content src=\"cover.xhtml\" />\n    </navPoint>\n%s</navMap>\n\n</ncx>\"\"\")",argv[argvurl],title,tocncx_navmap);	//end line
						
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
						
						sprintf(tocxhtml, "zf.writestr(\"toc.xhtml\", \"\"\"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">\n<head>\n<title>toc</title>\n<link href=\"template.css\" rel=\"stylesheet\" type=\"text/css\" />\n</head>\n\n<body>\n\n    <nav id=\"toc\" epub:type=\"toc\">\n        <h1 class=\"frontmatter\">Table of Contents</h1>\n        <ol class=\"contents\">\n            <li><a href=\"cover.xhtml\">%s</a></li>\n%s        </ol>\n\n    </nav>\n\n</body>\n\n</html>\"\"\")",title,toc_contents);
						
						//cover.xhtml
						
						char cover[strlen(title)*j + strlen(author) + strlen(info) + strlen(summary) + strlen(argv[argvurl]) + 590];
						sprintf(cover, "zf.writestr(\"cover.xhtml\", \"\"\"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\">\n<head>\n<title>%s</title>\n<link href=\"template.css\" rel=\"stylesheet\" type=\"text/css\" />\n</head>\n\n<body>\n\n    <h1><a href=\"%s\">%s</a></h1>\n    <h3>By %s</h3>\n    <br />\n    <pre>%sPublished %s\nUpdated %s\nDownloaded with <a href=\"https://github.com/Paranoid-Dev/ParanoidFFD\">ParanoidFFD</a>, %s</pre>\n    <br />\n    <p>%s</p>\n\n</body>\n</html>\"\"\")",title,argv[argvurl],title,author,info,published,updated,mark,summary);
						
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
					PyRun_SimpleString("chapter = re.sub(r'<div.*?</div>',r'',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub(r'<p[^>]*?(center|left|right|justify).*?>',r'<p style=\"text-align: \\1; text-align-last: \\1;\">',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub(r'<p (?![^>]*\\b(?:center|right|left|justify)\\b).*?>',r'<p>',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub(r'(<hr|<br).*?>',r'\\1 />',chapter, flags=re.DOTALL)");
					PyRun_SimpleString("chapter = re.sub(r'(<area|<base|<col|<embed|<img|<input|<link|<meta|<param|<source|<track|<wbr).*?>',r'',chapter, flags=re.DOTALL)");
					
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