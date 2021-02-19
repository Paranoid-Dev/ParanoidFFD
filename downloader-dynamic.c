#include <Python.h>	//	>= Python3.6
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <unistd.h>

const char* chapter[2048];
char* chaptername[2048];
const char *title, *author, *summary, *info, *filename;
PyObject *mainModule;
int i, j;
int f = 1;
int fname = 1;

int download () {
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
	return 0;
}

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

int writefile () {
	printf("writing to file...\n");
	int b = 1;
	size_t l;
	l = strlen(title) + strlen(author) + strlen(info) + strlen(summary) + 200; //+101+88+1(??)
	
	while (b <= j) {
		l = l + strlen(chaptername[b]) + 2;
		b = b + 1;
	}
	b = 1;
	while (b <= j) {
		l = l + strlen(chaptername[b]) + strlen(chapter[b]) + 12;
		b = b + 1;
	}
	b = 1;
	
	char buf1[l];
	char buf2[l];
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
	//printf("%s\n", buf1);

	if (fname == 1) {	//using title as filename (default)
		filename = title;
	}
	
	if (f == 1) {	//write to txt
		printf("saving to \"%s.txt\"\n",filename);
	//	sprintf(buf2, "cat << '492e9a7f651399f7a025c22d12e73a55' > \"%s.txt\"\n%s\n492e9a7f651399f7a025c22d12e73a55",filename,buf1);	//delimiter : ParanoidFFD MD5 hash - if some fanfiction contains this hash (which is very unlikely), it will fail to write
	//	system(buf2);
		FILE *fp;
		char output[strlen(filename)+5];
		sprintf(output, "%s.txt",filename);
		fp = fopen(output, "w+");
		fprintf(fp, "%s",buf1);
		fclose(fp);
	}
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
	printf("                 Paranoid FanFiction Downloader v1.1.0.2  by Paranoid-Dev                 \n");
	printf("                       https://github.com/Paranoid-Dev/ParanoidFFD                        \n");
	printf(" ________________________________________________________________________________________ \n");
	printf("                                                                                          \n");
	printf(" Usage    : ParanoidFFD <options>                                                         \n");
	printf("                                                                                          \n");
	printf(" Options  :                                                                               \n");
	printf("   -u <fanfiction url> : download from <fanfiction url>                                   \n");
	printf("   -f <FORMAT>         : save fanfiction as <FORMAT>                                      \n");
	printf("                         currently supported format : txt                                 \n");
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
	printf("   ParanoidFFD -f txt -o \"my fanfiction save\" -u \"my/fan/fiction/url\"                     \n");	//extra spaces are needed for uniform output.
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
		printf("                 Paranoid FanFiction Downloader v1.1.0.2  by Paranoid-Dev                 \n");
		printf("                       https://github.com/Paranoid-Dev/ParanoidFFD                        \n");
		printf(" ________________________________________________________________________________________ \n");
		printf(" \"ParanoidFFD --help\" to show help page                                                 \n");
		printf("                                                                                          \n");
	}
	else {
		while (p < argc) {
			if (strcmp(argv[p], "--version") == 0) {
				printf("ParanoidFFD 1.1.0.2\n");
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
				else {
					printf("invalid format : %s", argv[p]);
					//printf("ctrl + c to quit\n");
					//pause ();
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
				#ifdef _WIN32	//for Windows
					remove("chromedriver.exe");	//removing chromedriver if previous run crashed before deletion
				#else
					remove("chromedriver");
				#endif
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
				PyRun_SimpleString("chrome.get('https://raw.githubusercontent.com/Paranoid-Dev/ParanoidFFD/main/updates%20history/1.1.0.2-n')");
				PyRun_SimpleString("nextver = chrome.find_element_by_xpath('/html/body/pre').text");
				PyObject *nextverPy = PyObject_GetAttrString(mainModule, "nextver");
				const char * nextver = PyUnicode_AsUTF8(nextverPy);
				if (strcmp(nextver, "NA") == 0) {
					printf("ParanoidFFD is up to date! ParanoidFFD v1.1.0.2 by Paranoid-Dev\n");
				}
				else {
					printf("Current version : ParanoidFFD 1.1.0.2\nNew version : \n\n");
					PyRun_SimpleString("chrome.get(nextver)");
					PyRun_SimpleString("print(chrome.find_element_by_xpath('/html/body/pre').text)");
				}
				PyRun_SimpleString("chrome.quit()");
				Py_Finalize();
				//system("rm -f chromedriver");
				#ifdef _WIN32	//for Windows
					remove("chromedriver.exe");
				#else
					remove("chromedriver");
				#endif
			}
			else {
				printf("invalid argument : %s \n", argv[p]);
				//printf("ctrl + c to quit\n");
				//pause ();
			}
			p = p + 1;
		}
		
		if (down == 1) {
			printf("Downloading %s...\n",argv[argvurl]);
			
			#ifdef _WIN32	//for Windows
				remove("chromedriver.exe");	//removing chromedriver if previous run crashed before deletion
			#else
				remove("chromedriver");
			#endif
			
			char buf[l];
			sprintf(buf, "yurl = \"%s\"",argv[argvurl]);
			
			Py_Initialize();
			PyRun_SimpleString("import undetected_chromedriver as uc");
			PyRun_SimpleString("import random");
			PyRun_SimpleString("from time import sleep");
			
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
		
			while (1) {
				download ();
				if (i == j) {
					//	cleaning up before exiting
					printf("Download finished\n");
					PyRun_SimpleString("chrome.quit()");
					Py_Finalize();
					//system("rm -f chromedriver");
					#ifdef _WIN32	//for Windows
						remove("chromedriver.exe");
					#else
						remove("chromedriver");
					#endif
					printf("Cleaning up.. - removed chromedriver\n");
					writefile ();
					printf("Finished!\n");
					break;
				}
				PyRun_SimpleString("i += 1");
				PyRun_SimpleString("url = f\"https://www.{burl}/{i}\"");
			}
		}
	}
	return 0;
}