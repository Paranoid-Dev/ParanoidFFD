import re

import undetected_chromedriver as uc
options = uc.ChromeOptions()
options.headless = True
options.add_argument('--headless')
chrome = uc.Chrome(options=options)
chrome.get("<add url>")
chapter = chrome.find_element_by_xpath('//*[@id="storytext"]').get_attribute("innerHTML")
chapter = re.sub(r'<div.*?</div>',r'',chapter, flags=re.DOTALL)
chapter = re.sub(r'<p[^>]*?(center|left|right|justify).*?>',r'<p style="text-align: \1; text-align-last: \1;">',chapter, flags=re.DOTALL)
chapter = re.sub(r'<p (?![^>]*\b(?:center|right|left|justify)\b).*?>',r'<p>',chapter, flags=re.DOTALL)
chapter = re.sub(r'<hr.*?>',r'<hr />',chapter, flags=re.DOTALL)
chapter = re.sub(r'<br.*?>',r'<br />',chapter, flags=re.DOTALL)
print(chapter)