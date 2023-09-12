import sys
from PyQt5 import QtCore, QtGui, QtWidgets, uic
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QLabel, QLineEdit, QPushButton, QComboBox, QMessageBox
from PyQt5.QtGui import QColor, QIcon
from path import findHamiltonianPath, findHamiltonianPathL, findHamiltonianPathC, findHamiltonianPathF, findHamiltonianPathE

from path import findHamiltonianCycle, findHamiltonianCycleL, findHamiltonianCycleC, findHamiltonianCycleF, findHamiltonianCycleE

class MainWindow(QtWidgets.QMainWindow):
	def __init__(self):
		super().__init__()
		
		self.help = QPushButton(self, clicked = self.display_help)
		self.help.move(85, 350)
		self.help.resize(50, 50)
		self.help.setIcon(QIcon('help.png'))
		
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
		
		self.comboBox_algorithm = QComboBox(self)
		self.comboBox_algorithm.addItems(
		["Rectangle path", "Rectangle cycle",
		"L path", "L cycle",
		"C path", "C cycle",
		"F path", "F cycle",
		"E path", "E cycle"]
		)
		self.comboBox_algorithm.move(60, 200)
		self.comboBox_algorithm.adjustSize()		

		self.button_initiate = QPushButton(self, clicked = self.initiate_slides)
		self.button_initiate.setText("Start")
		self.button_initiate.resize(75, 25)
		self.button_initiate.move(75, 250)

		self.button_prev = QPushButton(self, clicked = self.previous_slide)
		self.button_next = QPushButton(self, clicked = self.next_slide)
		self.button_prev.setText("Previous")
		self.button_next.setText("Next")
		self.button_prev.resize(75, 25)
		self.button_next.resize(75, 25)
		self.button_prev.move(25, 300)
		self.button_next.move(125, 300)
        
		self.m = None
		self.n = None
		self.s = None
		self.t = None
		self.currentPoint = self.s
		self.algorithm = findHamiltonianPath
		canvas = QtGui.QPixmap(700, 600)
		canvas.fill(Qt.white)
		self.label.setPixmap(canvas)
		self.label.adjustSize()
		
	def initiate_slides(self):
		match self.comboBox_algorithm.currentIndex():
			case 0:
				self.algorithm = findHamiltonianPath
			case 1:
				self.algorithm = findHamiltonianCycle
			case 2:
				self.algorithm = findHamiltonianPathL
			case 3:
				self.algorithm = findHamiltonianCycleL
			case 4:
				self.algorithm = findHamiltonianPathC
			case 5:
				self.algorithm = findHamiltonianCycleC
			case 6:
				self.algorithm = findHamiltonianPathF
			case 7:
				self.algorithm = findHamiltonianCycleF
			case 8:
				self.algorithm = findHamiltonianPathE
			case _:
				self.algorithm = findHamiltonianCycleE
		
		self.label.pixmap().fill(Qt.white)
		try:
			self.m = int(self.input_m.text())
			self.n = int(self.input_n.text())
			self.s = (int(self.input_s_y.text()), int(self.input_s_x.text()))
			if self.comboBox_algorithm.currentIndex() % 2 == 0:
				self.t = (int(self.input_t_y.text()), int(self.input_t_x.text()))
		except:
			alert_window = QMessageBox(self)
			alert_window.setWindowTitle("Warning")
			alert_window.setText("You have not entered the necessary parameters or your parameters are invalid!\n")
			button = alert_window.exec()
			
			if button == QMessageBox.Ok:
				print("OK")
					
			return
		
		self.currentPoint = self.s
		
		painter = QtGui.QPainter(self.label.pixmap())
		pen = QtGui.QPen()
		pen.setWidth(3)
		pen.setColor(Qt.black)
		painter.setPen(pen)

		if self.comboBox_algorithm.currentIndex() < 2:
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
			if self.comboBox_algorithm.currentIndex() % 2 == 0:
				brush.setColor(Qt.red)
				painter.setBrush(brush)
				painter.drawRect(start_m + step_m * self.t[1], start_n + step_n * self.t[0], step_m, step_n)
		
			# sencimo crna polja
			brush.setColor(Qt.black)
			brush.setStyle(Qt.BDiagPattern)
			painter.setBrush(brush)
			for i in range(self.n):
				for j in range(self.m):
					if (i + j) % 2 == 1:
						painter.drawRect(start_m + step_m * j, start_n + step_n * i, step_m, step_n)
					
			# oznacavamo peeling ako imamo path u R
			if self.comboBox_algorithm.currentIndex() == 0:
				brush.setStyle(Qt.FDiagPattern)
				brush.setColor(QColor(255, 0, 0, 127))
				painter.setBrush(brush)
				r1 = min(self.s[0], self.t[0]) - 1;
				r2 = max(self.s[0], self.t[0]);
				r3 = min(self.s[1], self.t[1]) - 1;
				r4 = max(self.s[1], self.t[1]);
				if r1 % 2 == 0:
					r1 -= 1
				if (self.n - r2) % 2 == 0:
					r2 += 1
				if r3 % 2 == 0:
					r3 -= 1
				if (self.m - r4) % 2 == 0:
					r4 += 1
		
				# M5 je odredjen kao oblast od r3 + 1 do r4, i r1 + 1 do r2
				# painter.drawRect(start_m + step_m * (r3 + 1), start_n + step_n * (r1 + 1), step_m * (r4 - r3), step_n * (r2 - r1))
				# M1 je odredjen kao oblast od 0 do r3, i od 0 do self.n - 1
				if r3 > 0:
					brush.setColor(Qt.cyan)
					painter.setBrush(brush)
					painter.drawRect(start_m, start_n, step_m * (r3 + 1), step_n * self.n)
			
				# M3 je odredjen kao oblast od r3 + 1 do r4, i od 0 do r1
				if r1 > 0:
					brush.setColor(QColor(0, 0, 255))
					painter.setBrush(brush)
					painter.drawRect(start_m + step_m * (r3 + 1), start_n, step_m * (r4 - r3), step_n * (r1 + 1))
		
				# M4 je odredjen kao oblast od r3 + 1 do r4, i od r2 + 1 do self.n - 1
				if r4 < self.m:
					brush.setColor(Qt.yellow)
					painter.setBrush(brush)
					painter.drawRect(start_m + step_m * (r3 + 1), start_n + step_n * (r2 + 1), step_m * (r4 - r3), step_n * (self.n - r2 - 1))
		
				# M2 je odredjen kao oblast od r4 + 1 do self.m - 1, i od 0 do self.n - 1
				if r2 < self.n:
					brush.setColor(QColor(255, 0, 255))
					painter.setBrush(brush)
					painter.drawRect(start_m + step_m * (r4 + 1), start_n, step_m * (self.m - r4 - 1), step_n * self.n)
					
		elif self.comboBox_algorithm.currentIndex() < 6:
			# crtamo L
			m1 = 3 * self.m - 2
			n1 = 5 * self.n - 4
			step_n = 580 // (n1 + 1)
			step_m = 680 // (m1 + 1)
			start_n = 10 + (580 % (n1 + 1))
			start_m = 10 + (680 % (m1 + 1))
			start_n1 = start_n + step_n * (4 * self.n - 4)
			end_n1 = start_n + step_n * n1
			end_m1 = start_m + step_m * m1
			end_n = start_n + step_n * self.n
			end_m = start_n + step_m * self.m
			tmp_n = start_n
			tmp_m = start_m
			
			# crtamo levi krak slova L
			for j in range(self.m + 1):
				painter.drawLine(tmp_m, start_n, tmp_m, end_n1)
				tmp_m += step_m
			
			for j in range(2 * self.m - 2):
				painter.drawLine(tmp_m, start_n1, tmp_m, end_n1)
				tmp_m += step_m
				
			for i in range(4 * self.n - 4):
				painter.drawLine(start_m, tmp_n, end_m, tmp_n)
				tmp_n += step_n
			
			for i in range(self.n + 1):
				painter.drawLine(start_m, tmp_n, end_m1, tmp_n)
				tmp_n += step_n
			
			# crtamo gornji krak slova C
			tmp_m = start_m
			tmp_n = start_n
			if self.comboBox_algorithm.currentIndex() > 3:
				for j in range(3*self.m - 1):
					painter.drawLine(tmp_m, start_n, tmp_m, end_n)
					tmp_m += step_m
				
				for i in range(self.n + 1):
					painter.drawLine(start_m, tmp_n, end_m1, tmp_n)
					tmp_n += step_n
				
				
			# oznacavamo s
			brush = QtGui.QBrush()
			brush.setColor(Qt.green)
			brush.setStyle(Qt.SolidPattern)
			painter.setBrush(brush)
			painter.drawRect(start_m + step_m * self.s[1], start_n + step_n * self.s[0], step_m, step_n)
			# oznacavamo t
			if self.comboBox_algorithm.currentIndex() % 2 == 0:
				brush.setColor(Qt.red)
				painter.setBrush(brush)
				painter.drawRect(start_m + step_m * self.t[1], start_n + step_n * self.t[0], step_m, step_n)
		
			# sencimo crna polja
			brush.setColor(Qt.black)
			brush.setStyle(Qt.BDiagPattern)
			painter.setBrush(brush)
			for i in range(5*self.n - 4):
				for j in range(self.m):
					if (i + j) % 2 == 1:
						painter.drawRect(start_m + step_m * j, start_n + step_n * i, step_m, step_n)
			
			for i in range(4*self.n - 4, 5*self.n - 4):
				for j in range(self.m, 3*self.m - 2):
					if (i + j) % 2 == 1:
						painter.drawRect(start_m + step_m * j, start_n + step_n * i, step_m, step_n)
						
			if self.comboBox_algorithm.currentIndex() > 3:
				for i in range(self.n):
					for j in range(self.m, 3*self.m - 2):
						if (i + j) % 2 == 1:
							painter.drawRect(start_m + step_m * j, start_n + step_n * i, step_m, step_n)
							
		else:
			m1 = 3 * self.m - 2
			n1 = 5 * self.n - 4
			step_n = 580 // (n1 + 1)
			step_m = 680 // (m1 + 1)
			start_n = 10 + (580 % (n1 + 1))
			start_m = 10 + (680 % (m1 + 1))
			start_n1 = start_n + step_n * (4 * self.n - 4)
			start_small_n = start_n + step_n * (2*self.n - 2)
			end_small_n = start_n + step_n * (3*self.n - 2)
			end_small_m = start_m + step_m * (3*self.m - 4)
			end_n1 = start_n + step_n * n1
			end_m1 = start_m + step_m * m1
			end_n = start_n + step_n * self.n
			end_m = start_n + step_m * self.m
			tmp_n = start_n
			tmp_m = start_m
			
			for j in range(self.m + 1):
				painter.drawLine(tmp_m, start_n, tmp_m, end_n1)
				tmp_m += step_m
				
			for j in range(2*self.m - 2):
				painter.drawLine(tmp_m, start_n, tmp_m, end_n)
				if self.comboBox_algorithm.currentIndex() > 7:
					painter.drawLine(tmp_m, start_n1, tmp_m, end_n1)
				if j < 2*self.m - 4:
					painter.drawLine(tmp_m, start_small_n, tmp_m, end_small_n)
				tmp_m += step_m
			
			for i in range(self.n + 1):
				painter.drawLine(start_m, tmp_n, end_m1, tmp_n)
				tmp_n += step_n
			
			for i in range(self.n - 3):
				painter.drawLine(start_m, tmp_n, end_m, tmp_n)
				tmp_n += step_n
			
			for i in range(self.n + 1):
				painter.drawLine(start_m, tmp_n, end_small_m, tmp_n)
				tmp_n += step_n
			
			if self.comboBox_algorithm.currentIndex() < 8:
				for i in range(2*self.n - 2):
					painter.drawLine(start_m, tmp_n, end_m, tmp_n)
					tmp_n += step_n
			else:
				for i in range(self.n - 3):
					painter.drawLine(start_m, tmp_n, end_m, tmp_n)
					tmp_n += step_n
					
				for i in range(self.n + 1):
					painter.drawLine(start_m, tmp_n, end_m1, tmp_n)
					tmp_n += step_n
				
			# oznacavamo s
			brush = QtGui.QBrush()
			brush.setColor(Qt.green)
			brush.setStyle(Qt.SolidPattern)
			painter.setBrush(brush)
			painter.drawRect(start_m + step_m * self.s[1], start_n + step_n * self.s[0], step_m, step_n)
			# oznacavamo t
			if self.comboBox_algorithm.currentIndex() % 2 == 0:
				brush.setColor(Qt.red)
				painter.setBrush(brush)
				painter.drawRect(start_m + step_m * self.t[1], start_n + step_n * self.t[0], step_m, step_n)
			
			brush.setColor(Qt.black)
			brush.setStyle(Qt.BDiagPattern)
			painter.setBrush(brush)
			# sencimo crna polja
			for i in range(5*self.n - 4):
				for j in range(self.m):
					if (i + j) % 2 == 1:
						painter.drawRect(start_m + step_m * j, start_n + step_n * i, step_m, step_n)
						
			for j in range(self.m, 3*self.m - 2):
				for i in range(self.n):
					if (i + j) % 2 == 1:
						painter.drawRect(start_m + step_m * j, start_n + step_n * i, step_m, step_n)
						
				for i in range(2*self.n - 2, 3*self.n - 2):
					if (i + j) % 2 == 1 and j < 3*self.m - 4:
						painter.drawRect(start_m + step_m * j, start_n + step_n * i, step_m, step_n)
			
			if self.comboBox_algorithm.currentIndex() > 7:
				for i in range(4*self.n - 4, 5*self.n - 4):
					for j in range(self.m, 3*self.m - 2):
						if (i + j) % 2 == 1:
							painter.drawRect(start_m + step_m * j, start_n + step_n * i, step_m, step_n)
			
				
		painter.end()
		self.label.update()
    	
	def previous_slide(self):
		return None
    	
	def isCycle(self):
		x = self.algorithm == findHamiltonianCycle or self.algorithm == findHamiltonianCycleL
		x = x or self.algorithm == findHamiltonianCycleC or self.algorithm == findHamiltonianCycleF
		x = x or self.algorithm == findHamiltonianCycleE
		return x
    	
	def next_slide(self):
		tmp = self.algorithm(self.m, self.n, self.currentPoint[1], self.currentPoint[0], self.s, self.t)
		if tmp[0] == -1 or tmp[1] == -1:
			if self.currentPoint[0] == self.s[0] and self.currentPoint[1] == self.s[1]:
				alert_window = QMessageBox(self)
				alert_window.setWindowTitle("Warning")
				alert_window.setText("There is no Hamiltonian path/cycle in this case!\n Please, choose other parameters!\n")
				button = alert_window.exec()

				if button == QMessageBox.Ok:
					print("OK")
					
				return
			
			painter = QtGui.QPainter(self.label.pixmap())
			brush = QtGui.QBrush()
			brush.setColor(Qt.green)
			brush.setStyle(Qt.SolidPattern)
			painter.setBrush(brush)
			step_n = 580 // (self.n + 1)
			step_m = 680 // (self.m + 1)
			start_n = 10 + (580 % (self.n + 1))
			start_m = 10 + (680 % (self.m + 1))
			if self.algorithm != findHamiltonianPath and self.algorithm != findHamiltonianCycle:
				m1 = 3 * self.m - 2
				n1 = 5 * self.n - 4
				step_n = 580 // (n1 + 1)
				step_m = 680 // (m1 + 1)
				start_n = 10 + (580 % (n1 + 1))
				start_m = 10 + (680 % (m1 + 1))
			painter.drawRect(start_m + step_m * self.t[1], start_n + step_n * self.t[0], step_m, step_n)
			painter.end()
			self.label.update()
			return
			
		painter = QtGui.QPainter(self.label.pixmap())
		pen = QtGui.QPen()
		pen.setWidth(3)
		pen.setColor(Qt.green)
		painter.setPen(pen)
		step_n = 580 // (self.n + 1)
		step_m = 680 // (self.m + 1)
		start_n = 10 + (580 % (self.n + 1))
		start_m = 10 + (680 % (self.m + 1))
		
		if self.algorithm != findHamiltonianPath and self.algorithm != findHamiltonianCycle:
			m1 = 3 * self.m - 2
			n1 = 5 * self.n - 4
			step_n = 580 // (n1 + 1)
			step_m = 680 // (m1 + 1)
			start_n = 10 + (580 % (n1 + 1))
			start_m = 10 + (680 % (m1 + 1))
		painter.drawLine(start_m + step_m * self.currentPoint[1] + step_m // 2, start_n + step_n * self.currentPoint[0] + step_n // 2, start_m + step_m * tmp[1] + step_m // 2, start_n + step_n * tmp[0] + step_n // 2)
		self.currentPoint = tmp
		painter.end()
		self.label.update()
		
	def display_help(self):
		help_window = QMessageBox(self)
		help_window.setWindowTitle("Help")
		help_window.setText("This is an application for rendering Hamiltonian paths and Hamiltonian cycles in graphs of the following types:\n"
		+ "  1. rectangle\n  2. L\n  3. C\n  4. F\n  5. E\n"
		+ "Hamiltonian path is a path that traverses each vertex of the graph only once, starting at some point s and ending at some point t.\n"
		+ "Hamiltonian cycle is a path that traverses each vertex of the graph only once, starting at some point s and ending at that same point.\n"
		+ "Parameters m and n relate to dimensions of the graph in this way:\n"
		+ "  1. in rectangular graphs, m is the number of columns, n is the number of rows\n"
		+ "  2. in other types, the number of columns is 3m - 2, the number of rows is 5n - 4\n"
		+ "To run this application, enter the valid parameters and press Start to generate the initial slide.\n"
		+ "Afterwards, press Next to go to the next slide. If, at any point, you decide to go back, press Previous to return to that slide.\n"
		)
		
		help_window.adjustSize()
		button = help_window.exec()

		if button == QMessageBox.Ok:
			print("OK")

app = QtWidgets.QApplication(sys.argv)
window = MainWindow()
window.show()
app.exec_()
		
		
