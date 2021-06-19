



def morphEquivalentLength(path1, path2, fac):
    """
    Aims at converting paths of equivalent length by writing each operation in terms of the other. For example, an La,b in terms of Q
    """

import sys
from PyQt4 import QtGui, QtSvg

app = QtGui.QApplication(sys.argv) 
svgWidget = QtSvg.QSvgWidget('test.svg')
svgWidget.setGeometry(50,50,190,160)
svgWidget.show()

sys.exit(app.exec_())