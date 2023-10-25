import os
import requests

def download_file(url, save_path):
    response = requests.head(url)
    total_size = int(response.headers.get('content-length', 0))
    block_size = 1024  # 1 KB
    progress_bar_width = 40

    response = requests.get(url, stream=True)
    with open(save_path, 'wb') as file:
        progress_bar = '[' + ' ' * progress_bar_width + ']'
        print(f'Downloading {save_path}: {progress_bar}', end='\r')

        downloaded_size = 0
        for data in response.iter_content(block_size):
            file.write(data)
            downloaded_size += len(data)
            progress = int(progress_bar_width * downloaded_size / total_size)
            progress_bar = '[' + '#' * progress + ' ' * (progress_bar_width - progress) + ']'
            print(f'Downloading {save_path}: {progress_bar}', end='\r')

    print(f'Downloading {save_path}: Completed')

# 主文件的URL
main_file_url = 'https://kit-pro.fontawesome.com/releases/v6.4.2/css/pro.min.css'

# 创建文件夹
folder_name = '6.4.2 Pro'
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
        start_index = line.index('url(') + 4
        end_index = line.index(')', start_index)
        url = line[start_index:end_index]
        associated_files.append(url)

# 下载关联文件到与源文件相同的文件夹中
for file_url in associated_files:
    file_name = file_url.split('/')[-1]
    save_path = os.path.join(folder_name, file_name)
    download_file(file_url, save_path)
