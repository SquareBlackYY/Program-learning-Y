import os
import requests
import re
import time
import random

def download_file(url, save_path):
    response = requests.get(url, stream=True)
    block_size = 1024  # 1 KB

    with open(save_path, 'wb') as file:
        for data in response.iter_content(block_size):
            file.write(data)

# 版本号
version = '6.4.2'

# 主文件的URL
main_file_url = f'https://kit-pro.fontawesome.com/releases/v{version}/css/pro.min.css'

# 创建文件夹
folder_name = f'FontAwesome {version} Pro'
os.makedirs(folder_name, exist_ok=True)

# 下载主文件
main_file_path = os.path.join(folder_name, 'pro.min.css')
download_file(main_file_url, main_file_path)

# 解析主文件内容，查找关联文件
with open(main_file_path, 'r') as file:
    content = file.read()

# 提取关联文件的URL
associated_files = []
lines = content.split('\n')
for line in lines:
    if 'url(' in line:
        urls = re.findall(r'url\((.*?)\)', line)
        for url in urls:
            if url.startswith('../'):
                url = f'https://kit-pro.fontawesome.com/releases/v{version}/' + url[3:]
            associated_files.append(url)

# 随机化关联文件列表的顺序
random.shuffle(associated_files)

# 下载关联文件到与源文件相同的文件夹中
total_files = len(associated_files)
completed_files = 0

for file_url in associated_files:
    file_name = file_url.split('/')[-1]
    save_path = os.path.join(folder_name, file_name)
    download_file(file_url, save_path)
    completed_files += 1
    print(f'Downloaded file {completed_files}/{total_files}')

    # 添加延时
    time.sleep(random.uniform(0.5, 1.5))

print('Download completed')
