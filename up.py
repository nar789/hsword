import urllib.request
import re
import sys

r=re.compile(b'code=[^"]*')

page=urllib.request.urlopen('http://finance.naver.com/sise/sise_rise.nhn')
page=page.read()

datas=r.findall(page)

for i in datas:
    print(i.decode('utf-8')[5:])