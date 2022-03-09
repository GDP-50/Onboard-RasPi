
import sys

from PyQt5.QtCore import Qt, QPoint
from PyQt5.QtGui import QBrush, QPainter, QPen, QPolygonF
from PyQt5.QtWidgets import (
    QApplication,
    QGraphicsEllipseItem,
    QGraphicsItem,
    QGraphicsRectItem,
    QGraphicsPolygonItem,
    QGraphicsScene,
    QGraphicsView,
    QHBoxLayout,
    QPushButton,
    QSlider,
    QVBoxLayout,
    QWidget,
)

from parseCourse import loadCourse


class Window(QWidget):
    def __init__(self):
        super().__init__()
        self.green, self.bunkers = loadCourse("Hole 1.txt")

        # Defining a scene rect of 400x200, with it's origin at 0,0.
        # If we don't set this on creation, we can set it later with .setSceneRect
        self.scene = QGraphicsScene(0, 0, 400, 200)

      
        # Define the pen (line)
        polygon = QGraphicsPolygonItem(QPolygonF([QPoint(int(greenPoint[0] * 200), int(greenPoint[1] * 200)) for greenPoint in self.green]))
        pencil = QPen(Qt.green, 5)
        pencil.setStyle(Qt.SolidLine)
        brush = QBrush(Qt.green)
        polygon.setBrush(brush)
        polygon.setPen(pencil)
        self.scene.addItem(polygon)
        for bunker in self.bunkers:
            bunkerPoly = QGraphicsPolygonItem(QPolygonF([QPoint(int(bunkerPoint[0] * 200), int(bunkerPoint[1] * 200)) for bunkerPoint in bunker]))
            pencil = QPen(Qt.yellow, 5)
            pencil.setStyle(Qt.SolidLine)
            brush = QBrush(Qt.yellow)
            bunkerPoly.setBrush(brush)
            bunkerPoly.setPen(pencil)
            self.scene.addItem(bunkerPoly)
        view = QGraphicsView(self.scene)

        vbox = QVBoxLayout()

        dist = QPushButton("Distance")
        dist.clicked.connect(self.howFar)
        vbox.addWidget(dist)
        hbox = QHBoxLayout(self)
        hbox.addLayout(vbox)
        hbox.addWidget(view)

        self.setLayout(hbox)

    def howFar(self):
        print("distance is")

  



app = QApplication(sys.argv)

w = Window()
w.show()

app.exec()

    
    