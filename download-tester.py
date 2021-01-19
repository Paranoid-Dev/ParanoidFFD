import undetected_chromedriver as uc

options = uc.ChromeOptions()
options.headless = True
options.add_argument('--headless')
chrome = uc.Chrome(options=options)
url = input()
chrome.get(url)

with open("./out.txt", "w") as f:
    try:
        f.write(chrome.find_element_by_xpath('//*[@id=\"chap_select\"]').text)
    except:
        f.write(chrome.find_element_by_xpath('//*[@id=\"profile_top\"]/span[3]').text)
chrome.quit()