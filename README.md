# CppRoboticPhase2

ROS 2 воркспейс для системи технічного зору та керування робототехнічною платформою. Проєкт включає відстеження об'єктів на арені (ArUco маркери), детекцію перешкод та вузли для диференційного приводу (diff drive).

## 🛠 Технологічний стек та вимоги
* **ОС:** Linux Ubuntu 24.04
* **Фреймворк:** ROS 2 (Jazzy)
* **Мова:** C++
* **Бібліотеки:** OpenCV 4.x, lgpio

## 📦 Встановлення залежностей
Перед збиранням проєкту переконайтеся, що у вас встановлені необхідні бібліотеки:
```bash
sudo apt update
sudo apt install libopencv-dev liblgpio-dev

🚀 Збирання проєкту
Клонуйте репозиторій та зберіть його за допомогою colcon:
Bash
git clone [https://github.com/DexterWadpaker/CppRoboticPhase2.git](https://github.com/DexterWadpaker/CppRoboticPhase2.git)
cd CppRoboticPhase2
colcon build
(Щоб зібрати лише конкретний пакет, наприклад трекер арени, використовуйте: colcon build --packages-select arena_tracker_pkg)

🏃‍♂️ Запуск (на прикладі arena_tracker_node)
Після успішного збирання обов'язково активуйте оточення ROS 2:
Bash
source install/setup.bash
ros2 run arena_tracker_pkg arena_tracker_node
Щоб перевірити дані, які публікує вузол (наприклад, координати), відкрийте новий термінал, підтягніть оточення та виконайте:
Bash
ros2 topic list
ros2 topic echo /robot_position 
Важливо: Якщо ви використовуєте віртуальну машину (наприклад, OrbStack або Parallels на macOS) і отримуєте помилку підключення камери, обов'язково прочитайте файл TROUBLESHOOTING.md.

