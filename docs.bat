@echo off
git clone https://github.com/Troblecodings/Documentations
python docs.py
cd Documentations
git add .
git commit -m "Autoupdate docs"
git push
cd ..
rmdir /S /Q Documentations
pause