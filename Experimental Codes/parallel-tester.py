import undetected_chromedriver as uc
from joblib import Parallel, delayed

def fetch_chapter(url): 
    uc.install(executable_path='./chromedriver')
    options = uc.ChromeOptions()
    options.headless = True
    options.add_argument('--headless')
    chrome = uc.Chrome(options=options)
    print("downloading")
    print(url)
    chrome.get(url)
    summary = chrome.find_element_by_xpath('//*[@id="profile_top"]/span[4]').get_attribute("innerHTML")
    chrome.quit()
    return summary

urls = ['https://www.fanfiction.net/s/13540876/1/','https://www.fanfiction.net/s/10070079/','https://www.fanfiction.net/s/13540876/1/','https://www.fanfiction.net/s/10070079/']
summaries = Parallel(n_jobs=5)(delayed(fetch_chapter)(url) for url in urls)
print(summaries[0])
print(' ')
print(summaries[1])
print(' ')
print(summaries[2])
print(' ')
print(summaries[3])
print(' ')
print(summaries)