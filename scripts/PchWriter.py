# coding: utf-8

import os
import numpy as np
import logging

class PchWriter:
    LEVEL = logging.INFO

    def __init__(self, pchPath):
        self.logger = logging.getLogger(__name__)
        my_format = "logging:@:%(filename)s(%(lineno)s):fn:%(funcName)s:\nlevel:%(levelname)s:%(message)s"
        logging.basicConfig(level=self.LEVEL, format=my_format)    
        self.pchPath = pchPath
        self.wPchFile = open(self.pchPath, 'w')
        self.SEP = os.linesep
        self.STR_SPACE = " "
        self.volumeCount = 0
        self.isFirstTime = True

        #print("Initilized!sep")
    pass;


    def writeCoords(self, x,y,z):
        self.wPchFile.write(str(len(x))+self.SEP)
        for iTh in range(len(x)):
            self.wPchFile.write(str(x[iTh])+self.STR_SPACE)
            self.wPchFile.write(str(y[iTh])+self.STR_SPACE)
            self.wPchFile.write(str(z[iTh])+self.SEP)
        pass;
    pass;

    def appendCtivity(self, ctivity):
        if(self.isFirstTime):
            self.ctivity = ctivity
            self.isFirstTime = False
        else:
            self.ctivity = np.append(self.ctivity, ctivity)
        pass
    pass

    def writeCtivity(self):
        self.ctivity = self.ctivity.reshape((-1,3))
        self.wPchFile.write(str(len(self.ctivity))+self.SEP)
        for iTh in range(len(self.ctivity)):
            try:
                self.wPchFile.write(str(self.ctivity[iTh][0])+self.STR_SPACE)
                self.wPchFile.write(str(self.ctivity[iTh][1])+self.STR_SPACE)
                self.wPchFile.write(str(self.ctivity[iTh][2])+self.SEP)
            except Exception as e:
                self.logger.exception('cTvity.shape='+str(self.ctivity.shape)+':iTh='+str(iTh))
                self.logger.exception(e.args)
                exit()
            pass;
        pass;
        self.volumeCount += 1
    pass;

    def close(self):
        self.wPchFile.close()
pass;
