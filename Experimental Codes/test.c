#include <Python.h>	//	>= Python3.6
#include <stdio.h>
#include <stdlib.h>

#define SETUP_WEBDRIVER "from selenium import webdriver \n"\
						"import re \n"\
						"import random \n"\
						"import zipfile \n"\
						"import base64 \n"\
						"FLAG_ParanoidFFD_CHROMEID = f\"--ParanoidFFD_CHROMEID={random.randint(100000, 999999)}\" \n"\
						"options = webdriver.ChromeOptions() \n"\
						"options.binary_location = './.ParanoidFFD.ungoogled-chromium_91.0.4472.164-1.1.AppImage' \n"\
						"options.add_argument(FLAG_ParanoidFFD_CHROMEID) \n"\
						"options.add_argument('start-maximized') \n"\
						"options.add_experimental_option('excludeSwitches', ['enable-automation']) \n"\
						"options.add_argument('--disable-blink-features=AutomationControlled') \n"\
						"options.add_argument('--paranoidtesting1=0') \n"\
						"options.headless=True \n"\
						"options.add_argument('--headless') \n"\
						"chrome = webdriver.Chrome(options=options, executable_path='./.ParanoidFFD.chromedriver_v91') \n"\
						"ogagent = chrome.execute_script(\"return navigator.userAgent\") \n"\
						"chrome.execute_cdp_cmd(\"Network.setUserAgentOverride\", \n"\
						"    { \n"\
						"        \"userAgent\": ogagent.replace(\"Headless\", \"\"), \n"\
						"    }, \n"\
						"        ) \n"

PyObject *mainModule;
int exitcode = 0;

int initializeFFD () {
	Py_Initialize();
	mainModule = PyImport_AddModule("__main__");
	return PyRun_SimpleString(SETUP_WEBDRIVER);
}

int main () {

	while (initializeFFD ()) {
		puts("\n\nParanoidFFD intialization error - Trying again..\n\n");
		PyRun_SimpleString("chrome.quit()");
		Py_Finalize();
		exitcode++;
		if (exitcode == 10) {
			puts("\nParanoidFFD failed to initialize");
			exit(1);
		}
	}
	exitcode = 0;
	puts("ParanoidFFD initialized!");
	
/*	PyRun_SimpleString(
		"print(FLAG_ParanoidFFD_CHROMEID)\n"
"chrome.get('https://www.fanfiction.net/s/13540876/1/A-Simple-Act-of-Vengeance')\n"

"try: imagehtml = chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span[1]/img').get_attribute(\"outerHTML\")\n"
"except: imagehtml = 'no cover image'\n"


"imagelink = re.sub(r'.*?/image/(.*?)/75.*',r'https://www.fanfiction.net/image/\\1/180/',imagehtml, flags=re.DOTALL)\n"
"chrome.get(imagelink)\n"
"chrome.set_window_size('180', '240')\n"
"coverbase64 = chrome.get_screenshot_as_base64()\n"

"zf = zipfile.ZipFile(\"pre-ParanoidFFD_Template.epub\", mode=\"a\", compression=zipfile.ZIP_DEFLATED)\n"
"zf.writestr(\"cover.png\", base64.b64decode(coverbase64))\n"
"zf.close()\n"

"chrome.quit()\n"
		"print(len(imagehtml)) \n"

					   );
					   */
	PyObject *imagehtmlPy = PyObject_GetAttrString(mainModule, "imagehtml");
	int size = (int) PyObject_Size(imagehtmlPy);
	printf("%d\n", size);
	Py_Finalize();
	return 0;
}