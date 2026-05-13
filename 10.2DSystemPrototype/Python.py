import paho.mqtt.client as mqtt

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import QTimer

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1000, 713)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.Title = QtWidgets.QLabel(self.centralwidget)
        self.Title.setGeometry(QtCore.QRect(0, 10, 1000, 250))
        self.Title.setObjectName("Title")

        self.userStatusTxt = QtWidgets.QLabel(self.centralwidget)
        self.userStatusTxt.setGeometry(QtCore.QRect(110, 350, 281, 61))
        self.userStatusTxt.setObjectName("userStatusTxt")

        self.testSystemsBtn = QtWidgets.QPushButton(self.centralwidget)
        self.testSystemsBtn.setGeometry(QtCore.QRect(60, 250, 341, 51))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.testSystemsBtn.setFont(font)
        self.testSystemsBtn.setIconSize(QtCore.QSize(16, 16))
        self.testSystemsBtn.setObjectName("testSystemsBtn")

        self.systemSettingsBtn = QtWidgets.QPushButton(self.centralwidget)
        self.systemSettingsBtn.setGeometry(QtCore.QRect(430, 250, 341, 51))
        font = QtGui.QFont()
        font.setPointSize(18)
        self.systemSettingsBtn.setFont(font)
        self.systemSettingsBtn.setIconSize(QtCore.QSize(16, 16))
        self.systemSettingsBtn.setObjectName("systemSettingsBtn")

        self.quitBtn = QtWidgets.QPushButton(self.centralwidget)
        self.quitBtn.setGeometry(QtCore.QRect(740, 620, 101, 41))
        font = QtGui.QFont()
        font.setPointSize(14)
        self.quitBtn.setFont(font)
        self.quitBtn.setIconSize(QtCore.QSize(16, 16))
        self.quitBtn.setObjectName("quitBtn")

        self.userStatusLabel = QtWidgets.QLabel(self.centralwidget)
        self.userStatusLabel.setGeometry(QtCore.QRect(400, 350, 300, 61))
        self.userStatusLabel.setObjectName("userStatusLabel")

        self.objectDistanceTxt = QtWidgets.QLabel(self.centralwidget)
        self.objectDistanceTxt.setGeometry(QtCore.QRect(102, 450, 261, 61))
        self.objectDistanceTxt.setObjectName("objectDistanceTxt")

        self.distanceLabel = QtWidgets.QLabel(self.centralwidget)
        self.distanceLabel.setGeometry(QtCore.QRect(400, 450, 300, 61))
        self.distanceLabel.setObjectName("distanceLabel")

        self.connectionStatusTxt = QtWidgets.QLabel(self.centralwidget)
        self.connectionStatusTxt.setGeometry(QtCore.QRect(120, 550, 241, 41))
        self.connectionStatusTxt.setObjectName("connectionStatusTxt")

        self.connectionStatusLabel = QtWidgets.QLabel(self.centralwidget)
        self.connectionStatusLabel.setGeometry(QtCore.QRect(400, 540, 300, 61))
        self.connectionStatusLabel.setObjectName("connectionStatusLabel")

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 849, 33))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)

        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)


    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.Title.setText(_translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:56pt; font-weight:600; text-decoration: underline;\">Obstacle Detection<br/>System</span></p></body></html>"))
        self.userStatusTxt.setText(_translate("MainWindow", "<html><head/><body><p><span style=\" font-size:36pt;\">User Status:</span></p></body></html>"))
        self.testSystemsBtn.setText(_translate("MainWindow", "Test System "))
        self.systemSettingsBtn.setText(_translate("MainWindow", "System Settings"))
        self.quitBtn.setText(_translate("MainWindow", "Quit"))
        self.userStatusLabel.setText(_translate("MainWindow", "<html><head/><body><p><br/></p></body></html>"))
        self.objectDistanceTxt.setText(_translate("MainWindow", "<html><head/><body><p><span style=\" font-size:28pt;\">Object Distance:</span></p></body></html>"))
        self.distanceLabel.setText(_translate("MainWindow", "<html><head/><body><p><br/></p></body></html>"))
        self.connectionStatusTxt.setText(_translate("MainWindow", "<html><head/><body><p><span style=\" font-size:18pt;\">Arduino Device Status:</span></p></body></html>"))
        self.connectionStatusLabel.setText(_translate("MainWindow", "<html><head/><body><p><br/></p></body></html>"))


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        # Setup the user interface
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # Default labels
        self.ui.userStatusLabel.setText("SAFE")
        self.ui.distanceLabel.setText("- ")
        self.ui.connectionStatusLabel.setText("Disconnected")

        # Connect Quit button to close if clicked
        self.ui.quitBtn.clicked.connect(self.close)

        self.current_status = "SAFE"
        self.current_distance = "-"

        self.mqtt_client = mqtt.Client()
        self.mqtt_client.on_connect = self.on_mqtt_connect
        self.mqtt_client.on_message = self.on_mqtt_message

        try:
            self.mqtt_client.connect("localhost", 1883, 60)
            self.mqtt_client.loop_start()
            self.ui.connectionStatusLabel.setText("Connected to MQTT")
        except:
            self.ui.connectionStatusLabel.setText("MQTT Disconnected")


        def read_serial(self):
            if self.ser == None:
                return

        try:
            if self.ser.in_waiting:
                line = self.ser.readline().decode().strip()

                if "," in line:
                    status, distance = line.split(",")

                    # Update labels
                    self.ui.userStatusLabel.setText(status)
                    self.ui.distanceLabel.setText(distance + " cm")
                    self.ui.connectionStatusLabel.setText("Connected")

                    # Update colour
                    if status == "PROXIMITY DANGER":
                        self.ui.userStatusLabel.setStyleSheet("font-size: 28px; font-weight: bold; background-color: red; color: white;")
                
                    elif status == "PROXIMITY WARNING":
                        self.ui.userStatusLabel.setStyleSheet("font-size: 28px; font-weight: bold; background-color: yellow; color: black;")

                    else:
                        self.ui.userStatusLabel.setStyleSheet("font-size: 28px; font-weight: bold; background-color: lightgreen; color: black;")
        except:
            # If the Arduino disconnects
            self.ser = None
            self.ui.connectionStatusLabel.setText("Disconnected")


    def on_mqtt_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.ui.connectionStatusLabel.setText("Connected to MQTT")
            client.subscribe("vision/status")
            client.subscribe("vision/distance")
        else:
            self.ui.connectionStatusLabel.setText("MQTT Failed")


    def on_mqtt_message(self, client, userdata, message):
        topic = message.topic
        payload = message.payload.decode()

        if topic == "vision/status":
            self.current_status = payload

        elif topic == "vision/distance":
            self.current_distance = payload

        self.update_gui()


    def update_gui(self):
        self.ui.userStatusLabel.setText(self.current_status)
        self.ui.distanceLabel.setText(self.current_distance + " cm")
        self.ui.connectionStatusLabel.setText("Connected to MQTT")

        if self.current_status == "PROXIMITY DANGER":
            self.ui.userStatusLabel.setStyleSheet(
                "font-size: 28px; font-weight: bold; background-color: red; color: white;"
        )

        elif self.current_status == "PROXIMITY WARNING":
            self.ui.userStatusLabel.setStyleSheet(
                "font-size: 28px; font-weight: bold; background-color: yellow; color: black;"
        )

        else:
            self.ui.userStatusLabel.setStyleSheet(
                "font-size: 28px; font-weight: bold; background-color: lightgreen; color: black;"
        )

    

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())


