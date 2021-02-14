"""
> python test/create_secrets.py
"""


secrets = {
    "char AWS_KEY[]": r'"$AWS_KEY"',
    "char AWS_SECRET[]": r'"$AWS_SECRET"',
    "char AWS_REGION[]": r'"$AWS_REGION"',
    "const char* AWS_TOPIC": r'"$AWS_TOPIC"',
    "int PORT": "$PORT",

    "const char WIFI_SSID[]": r'"$WIFI_SSID"',
    "const char WIFI_PASSWORD[]": r'"$WIFI_PASSWORD"',
    "const char AWS_IOT_ENDPOINT[]": r'"$AWS_IOT_ENDPOINT"',
}


with open("lib/connect/secrets_new.h",'w') as f:
    for k in secrets:
        f.write(f"{k} = {secrets[k]};\n")
