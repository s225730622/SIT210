from PyQt5 import QtCore, QtGui, QtWidgets
import RPi.GPIO as GPIO
import sys

GPIO.setmode(GPIO.BOARD)

# Define pins
LivingRoomPin = 32
BathroomPin = 13
ClosetPin = 15

GPIO.setup(LivingRoomPin, GPIO.OUT)
livingRoomPWM = GPIO.PWM(LivingRoomPin, 1000)
livingRoomPWM.start(0)
GPIO.setup(BathroomPin, GPIO.OUT)
GPIO.setup(ClosetPin, GPIO.OUT)


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(754, 600)
        palette = QtGui.QPalette()
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Base, brush)
        brush = QtGui.QBrush(QtGui.QColor(255, 255, 255))
        brush.setStyle(QtCore.Qt.SolidPattern)
        palette.setBrush(QtGui.QPalette.Active, QtGui.QPalette.Window, brush)
        MainWindow.setPalette(palette)
        MainWindow.setContextMenuPolicy(QtCore.Qt.DefaultContextMenu)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        # Title label
        self.Title = QtWidgets.QLabel(self.centralwidget)
        self.Title.setGeometry(QtCore.QRect(0, 20, 751, 101))
        font = QtGui.QFont()
        font.setPointSize(48)
        self.Title.setFont(font)
        self.Title.setAlignment(QtCore.Qt.AlignCenter)
        self.Title.setObjectName("Title")

        # Body label
        self.label2 = QtWidgets.QLabel(self.centralwidget)
        self.label2.setGeometry(QtCore.QRect(100, 350, 550, 120))
        font2 = QtGui.QFont()
        font2.setPointSize(20)
        self.label2.setFont(font2)
        self.label2.setAlignment(QtCore.Qt.AlignCenter)
        self.label2.setObjectName("label2")

        # Bathroom LED 
        self.Bathroom = QtWidgets.QRadioButton(self.centralwidget)
        self.Bathroom.setGeometry(QtCore.QRect(320, 180, 200, 50))
        font3 = QtGui.QFont()
        font3.setFamily("Segoe UI")
        font3.setPointSize(24)      
        font3.setWeight(75)
        self.Bathroom.setFont(font3)
        self.Bathroom.setObjectName("Bathroom")
        self.Bathroom.setAutoExclusive(False)
        
        # Closet LED 
        self.Closet = QtWidgets.QRadioButton(self.centralwidget)
        self.Closet.setGeometry(QtCore.QRect(560, 180, 200, 50))
        self.Closet.setFont(font3)
        self.Closet.setObjectName("Closet")
        self.Closet.setAutoExclusive(False)

        # Living Room LED
        self.LivingRoom = QtWidgets.QRadioButton(self.centralwidget)
        self.LivingRoom.setGeometry(QtCore.QRect(50, 180, 200, 50))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.LivingRoom.sizePolicy().hasHeightForWidth())
        self.LivingRoom.setSizePolicy(sizePolicy)
        font = QtGui.QFont()
        font.setFamily("Segoe UI")
        font.setPointSize(24)
        font.setBold(True)
        font.setItalic(False)
        font.setWeight(75)
        self.LivingRoom.setFont(font)
        self.LivingRoom.setObjectName("LivingRoom")
        self.LivingRoom.setAutoExclusive(False)

        # Living Room Slider
        self.livingRoomSlider = QtWidgets.QSlider(self.centralwidget)
        self.livingRoomSlider.setGeometry(QtCore.QRect(60, 300, 160, 22))
        self.livingRoomSlider.setSliderPosition(0)
        self.livingRoomSlider.setOrientation(QtCore.Qt.Horizontal)
        self.livingRoomSlider.setObjectName("livingRoomSlider")
        self.lRoomSliderLbl = QtWidgets.QLabel(self.centralwidget)
        self.lRoomSliderLbl.setGeometry(QtCore.QRect(90, 320, 101, 21))
        self.lRoomSliderLbl.setObjectName("lRoomSliderLbl")
        self.livingRoomSlider.setMinimum(0)
        self.livingRoomSlider.setMaximum(100)
        self.livingRoomSlider.setValue(1)
        self.livingRoomSlider.valueChanged.connect(self.updateLivingRoomBrightness)

        # Exit button
        self.exitButton = QtWidgets.QPushButton(self.centralwidget)
        self.exitButton.setGeometry(QtCore.QRect(270, 470, 200, 41))
        self.exitButton.setFont(font2)
        self.exitButton.setObjectName("exitButton")
        self.exitButton.pressed.connect(MainWindow.close) # type: ignore
        
        # Add LED images in
        self.blueOFF = QtWidgets.QLabel(self.centralwidget)
        self.blueOFF.setGeometry(QtCore.QRect(110, 240, 31, 51))
        self.blueOFF.setText("")
        self.blueOFF.setPixmap(QtGui.QPixmap("blueLedOFF.png"))
        self.blueOFF.setScaledContents(True)
        self.blueOFF.setObjectName("blueOFF")
        self.greenOFF = QtWidgets.QLabel(self.centralwidget)
        self.greenOFF.setGeometry(QtCore.QRect(380, 240, 31, 51))
        self.greenOFF.setLineWidth(0)
        self.greenOFF.setText("")
        self.greenOFF.setPixmap(QtGui.QPixmap("greenLedOFF.png"))
        self.greenOFF.setScaledContents(True)
        self.greenOFF.setObjectName("greenOFF")
        self.yellowOFF = QtWidgets.QLabel(self.centralwidget)
        self.yellowOFF.setGeometry(QtCore.QRect(600, 240, 31, 51))
        self.yellowOFF.setLineWidth(0)
        self.yellowOFF.setText("")
        self.yellowOFF.setPixmap(QtGui.QPixmap("yellowLedOFF.png"))
        self.yellowOFF.setScaledContents(True)
        self.yellowOFF.setObjectName("yellowOFF")
        self.blueON = QtWidgets.QLabel(self.centralwidget)
        self.blueON.setGeometry(QtCore.QRect(100, 230, 51, 61))
        self.blueON.setText("")
        self.blueON.setPixmap(QtGui.QPixmap("blueLedON.png"))
        self.blueON.setScaledContents(True)
        self.blueON.setObjectName("blueON")
        self.greenON = QtWidgets.QLabel(self.centralwidget)
        self.greenON.setGeometry(QtCore.QRect(370, 230, 51, 61))
        self.greenON.setText("")
        self.greenON.setPixmap(QtGui.QPixmap("greenLedON.png"))
        self.greenON.setScaledContents(True)
        self.greenON.setObjectName("greenON")
        self.yellowON = QtWidgets.QLabel(self.centralwidget)
        self.yellowON.setGeometry(QtCore.QRect(590, 230, 51, 61))
        self.yellowON.setText("")
        self.yellowON.setPixmap(QtGui.QPixmap("yellowLedON.png"))
        self.yellowON.setScaledContents(True)
        self.yellowON.setObjectName("yellowON")

        # Make sure the LED icon is always OFF to begin with
        self.blueON.hide()
        self.greenON.hide()
        self.yellowON.hide()

        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        # Checks if the living room light is on/off and calls the lightToggled method accordingly
        self.LivingRoom.toggled.connect(lambda checked: self.lightToggled(self.LivingRoom, "Living Room", checked))
        self.Bathroom.toggled.connect(lambda checked: self.lightToggled(self.Bathroom, "Bathroom", checked))
        self.Closet.toggled.connect(lambda checked: self.lightToggled(self.Closet, "Closet", checked))

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.Title.setText(_translate("MainWindow", "Linda\'s House Lights"))
        self.Bathroom.setText(_translate("MainWindow", "Bathroom"))
        self.Closet.setText(_translate("MainWindow", "Closet"))
        self.LivingRoom.setText(_translate("MainWindow", "Living Room"))
        self.exitButton.setText(_translate("MainWindow", "Exit GUI"))
        self.lRoomSliderLbl.setText(_translate("MainWindow", "Intensity Level"))

    # Method which hides either the LED OFF/LED ON icon depending on the state of the LED
    def updateLedImage(self):
        # Update Living Room LED icon
        self.blueON.setVisible(self.LivingRoom.isChecked())
        self.blueOFF.setVisible(not self.LivingRoom.isChecked())

        # Update Bathroom LED icon
        self.greenON.setVisible(self.Bathroom.isChecked())
        self.greenOFF.setVisible(not self.Bathroom.isChecked())

        # Update Closet LED icon
        self.yellowON.setVisible(self.Closet.isChecked())
        self.yellowOFF.setVisible(not self.Closet.isChecked())

    def updateLivingRoomBrightness(self, value):
        if self.LivingRoom.isChecked():
            livingRoomPWM.ChangeDutyCycle(value)
            self.label2.setText(f"Living Room Intensity: {value}%")
        else:
            livingRoomPWM.ChangeDutyCycle(0)

    def lightToggled(self, checkedBtn, room, checked):
        # To use in for loop
        ledBtns = [self.LivingRoom, self.Bathroom, self.Closet]

        if checked:
            # Create a for loop which iterates through each light which hasnt been toggled on and blocks toggle signals to turn them off 
            for ledBtn in ledBtns:
                if ledBtn is not checkedBtn:
                    ledBtn.blockSignals(True)
                    ledBtn.setChecked(False)
                    ledBtn.blockSignals(False)

            # Calls method which turns lights off
            self.turnLightsOff()

            if room == "Living Room":
                brightness = self.livingRoomSlider.value()
                livingRoomPWM.ChangeDutyCycle(brightness)
                GPIO.output(BathroomPin, GPIO.LOW)
                GPIO.output(ClosetPin, GPIO.LOW)
                self.label2.setText(f"{room} Light ON ({brightness}%)")

            elif room == "Bathroom":
                # Send ON signal to Raspberri Pi device
                GPIO.output(BathroomPin, GPIO.HIGH)
                livingRoomPWM.ChangeDutyCycle(0)
                GPIO.output(ClosetPin, GPIO.LOW)
                self.label2.setText(room + " Light ON")

            elif room == "Closet":
                # Send ON signal to Raspberri Pi device
                GPIO.output(ClosetPin, GPIO.HIGH)
                livingRoomPWM.ChangeDutyCycle(0)
                GPIO.output(BathroomPin, GPIO.LOW)
                self.label2.setText(room + " Light ON")

        else:
            self.turnLightsOff()
            self.label2.setText(room + " Light OFF")

        self.updateLedImage()

    # Method to turn all lights off
    def turnLightsOff(self):
        livingRoomPWM.ChangeDutyCycle(0)
        GPIO.output(BathroomPin, GPIO.LOW)
        GPIO.output(ClosetPin, GPIO.LOW)

    def close(self):
        GPIO.cleanup()


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())  
