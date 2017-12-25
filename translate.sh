#!/bin/bash
lupdate ./yourdroid.pro
sed '/source/! b;N;s/\(\([^>]*\)<\/.*\) type[^>]*>/\1>\2/' yourdroid_en.ts > yourdroid_en_temp.ts
cat yourdroid_en_temp.ts > yourdroid_en.ts
rm -f yourdroid_en_temp.ts
lrelease ./yourdroid.pro
echo ==== END ====
read -n 1
