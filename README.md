
PROJECT IOT102
Project name Easy Control Light



I. Introduction
This system helps turn on/off 220V lights simply by clapping your hands and learning to turn them on and off remotely via wifi. This system also measures the temperature when the temperature is too high in the room and will send a warning signal via wifi. It can be applied in smart homes, supporting the disabled to help users control the lights easily and ensure temperature safety in closed rooms.
II. Content
1.Block diagram (Sơ đồ khối )
(Vẽ sơ đồ khối, phân tích kết nối và vai trò của từng khối)
<img width="762" height="322" alt="image" src="https://github.com/user-attachments/assets/df69bb7a-15f2-4a4f-8ae5-f5957fe0aac6" />

2. Hardware Required (Linh Kiện)
• ESP32
•Sound Sensor
•LM35 Temperature Sensor
•6x6x10MM DIP 2-Pin Push Button
•Chip Buzzer
•5V Relay Module
•Wires, Cables, Circuit Boards
•220V Light Bulb

3. Electronic circuit (Mạch điện tử)
•
Mạch mô phỏng hoặc mạch thực tế. (Bắt buộc phải có)

<img width="770" height="495" alt="image" src="https://github.com/user-attachments/assets/f4c9f746-fa5a-4487-a1f5-8ecc9efbe820" />

<img width="715" height="537" alt="image" src="https://github.com/user-attachments/assets/c65cfb1e-b749-411f-bfc4-e3332fb02465" />


4. Flow chart (Lưu đồ thuật toán)
Lưu đồ giải thuật của hàm void loop (Bắt buộc phải có)

<img width="597" height="699" alt="image" src="https://github.com/user-attachments/assets/ec16cefb-6806-4eb7-ab7b-817197895ffa" />



III. Experiment
1.Prototype implementation results
	Kết quả triển khai phần cứng, mô hình chạy được theo mong muốn 
2.Functional results of the system
The function results show the target:
Light on/off function by push button and light on/off via Blynk
Light on/off via sound sensor, sound sensor function on/off via Blynk
High temperature warning via LM35 displayed on Blink when the buzzer sounds and the light flashes continuously

3.Conclude
The goal was achieved. Future development could be to control the level of the sound sensor to change according to user wishes on Blynk.
IV. Appendix
1. Link 
-Link mô phỏng 
https://app.cirkitdesigner.com/project/a923682d-8a18-4e14-8aac-590b6c8f5a36
-Link video (nếu có)
