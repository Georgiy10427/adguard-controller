1. После первичной активации выполните команду:  
```sudo apt upgrade```  
2. Установите FTP сервер для заливки прошивки:  
```sudo apt-get install proftpd```  
3. Подключитесь к плате по FTP (стандартно: логин: dietpi пароль: dietpi) и создайте каталог controller и перейдите в него. В него поместите файлы: директория web; run.sh; server_arm.bin  
4. Добавьте новую задачу в планировщик:  
```sudo crontab -e```  
Впишите в открывшийся файл строку ниже и сохраните его нажатие ctrl+x и y.
```@reboot sudo nohup /home/dietpi/controller/run.sh &```
Добавьте права на исполнение файлам командами:
```sudo chmod 777 /home/dietpi/controller/run.sh```
```sudo chmod 777 /home/dietpi/controller/server_arm.bin```
5. Перезагрузите устройство командой ```sudo reboot```