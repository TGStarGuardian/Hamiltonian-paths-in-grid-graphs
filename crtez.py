import sys
from PyQt5 import QtCore, QtGui, QtWidgets, uic
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QLabel, QLineEdit, QPushButton

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        self.label = QLabel(self)
        self.label.move(250, 50)
        self.setGeometry(0, 0, 1000, 700)
        self.input_s_x = QLineEdit(self)
        self.input_s_y = QLineEdit(self)
        self.input_t_x = QLineEdit(self)
        self.input_t_y = QLineEdit(self)
        self.input_m = QLineEdit(self)
        self.input_n = QLineEdit(self)
        self.input_s_x.resize(50, 25)
        self.input_s_y.resize(50, 25)
        self.input_t_x.resize(50, 25)
        self.input_t_y.resize(50, 25)
        self.input_m.resize(50, 25)
        self.input_n.resize(50, 25)
        self.input_m.move(50, 50)
        self.input_n.move(150, 50)
        self.input_s_x.move(50, 100)
        self.input_s_y.move(150, 100)
        self.input_t_x.move(50, 150)
        self.input_t_y.move(150, 150)
        self.label_s_x = QLabel(self)
        self.label_s_y = QLabel(self)
        self.label_t_x = QLabel(self)
        self.label_t_y = QLabel(self)
        self.label_m = QLabel(self)
        self.label_n = QLabel(self)
        self.label_s_x.move(25, 100)
        self.label_s_y.move(125, 100)
        self.label_t_x.move(25, 150)
        self.label_t_y.move(125, 150)
        self.label_m.move(25, 50)
        self.label_n.move(125, 50)
        self.label_s_x.setText("s.x:")
        self.label_s_y.setText("s.y")
        self.label_t_x.setText("t.x:")
        self.label_t_y.setText("t.y:")
        self.label_m.setText("m:")
        self.label_n.setText("n:")
        self.label_s_x.adjustSize()
        self.label_s_y.adjustSize()
        self.label_t_x.adjustSize()
        self.label_t_y.adjustSize()
        self.label_m.adjustSize()
        self.label_n.adjustSize()
        
        self.button_initiate = QPushButton(self, clicked = self.initiate_slides)
        self.button_initiate.setText("Start")
        self.button_initiate.resize(75, 25)
        self.button_initiate.move(75, 200)
        
        self.button_prev = QPushButton(self, clicked = self.previous_slide)
        self.button_next = QPushButton(self, clicked = self.next_slide)
        self.button_prev.setText("Previous")
        self.button_next.setText("Next")
        self.button_prev.resize(75, 25)
        self.button_next.resize(75, 25)
        self.button_prev.move(25, 250)
        self.button_next.move(125, 250)
        
        self.m = None
        self.n = None
        self.s = None
        self.t = None
        
        canvas = QtGui.QPixmap(700, 600)
        canvas.fill(Qt.white)
        #self.setCentralWidget(self.label)
        self.label.setPixmap(canvas)
        self.label.adjustSize()
        #self.draw_something()

    def draw_something(self):
        painter = QtGui.QPainter(self.label.pixmap())
        painter.drawLine(10, 10, 300, 200)
        painter.end()
        
    def initiate_slides(self):
    	self.label.pixmap().fill(Qt.white)
    	self.m = int(self.input_m.text())
    	self.n = int(self.input_n.text())
    	self.s = (int(self.input_s_y.text()), int(self.input_s_x.text()))
    	self.t = (int(self.input_t_y.text()), int(self.input_t_x.text()))
    	painter = QtGui.QPainter(self.label.pixmap())
    	pen = QtGui.QPen()
    	pen.setWidth(3)
    	pen.setColor(Qt.black)
    	painter.setPen(pen)

    	step_n = 580 // (self.n + 1)
    	step_m = 680 // (self.m + 1)
    	start_n = 10 + (580 % (self.n + 1))
    	start_m = 10 + (680 % (self.m + 1))
    	end_n = start_n + step_n * self.n
    	end_m = start_m + step_m * self.m
    	tmp_n = start_n
    	tmp_m = start_m

	# crtamo horizontalne linije
    	for i in range(self.n + 1):
    		painter.drawLine(start_m, tmp_n, end_m, tmp_n)
    		tmp_n += step_n
    		
	# crtamo vertikalne linije
    	for j in range(self.m + 1):
    		painter.drawLine(tmp_m, start_n, tmp_m, end_n)
    		tmp_m += step_m
    		
    	# polje ima velicinu (step_m, step_n)
    	# levi ugao od (x, y) ima koordinate
    	# (start_m + step_m * x, start_n + step_n * y)
    	# oznacavamo s
    	brush = QtGui.QBrush()
    	brush.setColor(Qt.green)
    	brush.setStyle(Qt.SolidPattern)
    	painter.setBrush(brush)
    	painter.drawRect(start_m + step_m * self.s[1], start_n + step_n * self.s[0], step_m, step_n)
    	# oznacavamo t
    	brush.setColor(Qt.red)
    	painter.setBrush(brush)
    	painter.drawRect(start_m + step_m * self.t[1], start_n + step_n * self.t[0], step_m, step_n)
    	
    	painter.end()
    	self.label.update()
    	
    def previous_slide(self):
    	return None
    	
    def next_slide(self):
    	return None

app = QtWidgets.QApplication(sys.argv)
window = MainWindow()
window.show()
app.exec_()
