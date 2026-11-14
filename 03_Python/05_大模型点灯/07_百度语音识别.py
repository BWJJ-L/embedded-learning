import base64
import urllib
import requests
import json
import os

api_key = os.getenv("BAIDU_API_KEY")
if not api_key:
    raise ValueError("环境变量 BAIDU_API_KEY 未设置")

def main():
        
    url = "https://vop.baidu.com/pro_api"
    
    # speech 可以通过 get_file_content_as_base64("C:\fakepath\audio_record.pcm",False) 方法获取
    payload = json.dumps({
        "format": "pcm",
        "rate": 16000,
        "channel": 1,
        "cuid": "AGNUaGlBEhZFDeGHQakeK9z0vqbHxRPs",
        "token": "",
        "dev_pid": 80001,
        "speech": get_file_content_as_base64("./audio_record.pcm",False),
        "len": os.path.getsize()
    }, ensure_ascii=False)
    headers = {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
        'Authorization': f'Bearer {api_key}'
        
    }
    
    response = requests.request("POST", url, headers=headers, data=payload.encode("utf-8"))
    
    response.encoding = "utf-8"
    print(response.text)
    

def get_file_content_as_base64(path, urlencoded=False):
    """
    获取文件base64编码
    :param path: 文件路径
    :param urlencoded: 是否对结果进行urlencoded 
    :return: base64编码信息
    """
    with open(path, "rb") as f:
        content = base64.b64encode(f.read()).decode("utf8")
        if urlencoded:
            content = urllib.parse.quote_plus(content)
    return content

if __name__ == '__main__':
    main()
