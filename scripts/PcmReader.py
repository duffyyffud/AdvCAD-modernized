# coding: utf-8

import numpy as np
import logging

logger = logging.getLogger(__name__)
class PcmReader:
    def __init__(self, pchPath):
        self.pchPath = pchPath
        self.rPcmFile = open(self.pchPath, 'r')
        #print("Initilized!sep")
    pass;

    def read(self):
        N_NODES_COL = 0
        N_VOLUMES_COL = 2
        N_TRIS_COL = 0
        VOLUME_ID_COL = 2
        X_COL = 0
        Y_COL = 1
        Z_COL = 2
        pcmLines = self.rPcmFile.readlines()
        tokens = pcmLines[0].split()
        logger.info("Line 0: %r  tokens=%s len=%d", pcmLines[0].rstrip(), tokens, len(tokens))
        self.nNodes = -2#initial value
        self.nNodes = int(tokens[N_NODES_COL])
        self.nVolumes = int(tokens[N_VOLUMES_COL])
        self.cumulativeNtris = np.zeros(self.nVolumes,dtype=int)
        for iTh in range(self.nVolumes):
            tokens = pcmLines[self.nNodes+1].split()
            logger.info("PreScan vol[%d]: reading pcmLines[%d]=%r  tokens=%s len=%d",
                        iTh, self.nNodes+1, pcmLines[self.nNodes+1].rstrip(), tokens, len(tokens))
            if(iTh == 0):
                self.cumulativeNtris[iTh] = int(tokens[N_TRIS_COL])
            else:
                self.cumulativeNtris[iTh] = self.cumulativeNtris[iTh-1] + int(tokens[N_TRIS_COL])
            pass;
        pass;
        
        iLine=0
        iNode=0
        iTri = 0
        iVolume = 0
        self.ctivity = np.ndarray((self.cumulativeNtris[len(self.cumulativeNtris)-1],3),dtype=int)
        for line in pcmLines:
            tokens = line.split()
            if(iLine == 0):
                #print ('tokens='+ str(tokens))
                #1st line is nNodes dummy nVolumes
                print ('nNodes ='+ str(self.nNodes))
                print ('nVolumes ='+ str(self.nVolumes))
                self.x = np.zeros(self.nNodes)
                self.y = np.zeros(self.nNodes)
                self.z = np.zeros(self.nNodes)
                lineIdNextBeginVolume = self.nNodes+1
            elif(iLine <= self.nNodes):
                self.x[iNode] = float(tokens[X_COL])
                self.y[iNode] = float(tokens[Y_COL])
                self.z[iNode] = float(tokens[Z_COL])
                iNode += 1
            elif(iLine == lineIdNextBeginVolume):
                logger.info("VolumeHeader: iLine=%d  line=%r  tokens=%s len=%d",
                            iLine, line.rstrip(), tokens, len(tokens))
                nTris = int(tokens[N_TRIS_COL])
                if len(tokens) > VOLUME_ID_COL:
                    volumeId = int(tokens[VOLUME_ID_COL])
                else:
                    volumeId = iVolume
                tempCTivity = np.zeros((nTris, 3),dtype=int)
                np.append(self.ctivity, tempCTivity)
                self.ctivity.reshape((self.cumulativeNtris[volumeId],3))
                lineIdNextBeginVolume = iLine + nTris + 1
                iTri = 0
                iVolume += 1
            else:
                try:
                    if(volumeId == 0):
                        self.ctivity[iTri][0] = int(tokens[0])
                        self.ctivity[iTri][1] = int(tokens[1])
                        self.ctivity[iTri][2] = int(tokens[2])
                    else:
                        self.ctivity[self.cumulativeNtris[volumeId-1] + iTri][0] = int(tokens[0])
                        self.ctivity[self.cumulativeNtris[volumeId-1] + iTri][1] = int(tokens[1])
                        self.ctivity[self.cumulativeNtris[volumeId-1] + iTri][2] = int(tokens[2])
                    pass;
                except IndexError:
                    print("Error:self.ctivity.shape="+ str(self.ctivity.shape))
                    print("Error:ctivity index out of bounds error.:iTri="+ str(iTri)+":volumeId="+ str(volumeId))
                    quit()
                pass;
                iTri += 1
            pass;
            iLine += 1
        pass;
        #print(self.ctivity)
        self.rPcmFile.close()
        assert (iNode == self.nNodes), "iNode="+iNode +":not agree with self.nNodes="+self.nNodes
        assert (iTri == nTris), "iNode="+iTri +":not agree with nNodes="+nTris
        return self.x, self.y, self.z, self.ctivity, self.cumulativeNtris
    pass;

    def get_nNodes(self):
        return self.nNodes
    pass;

if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG, format='%(levelname)s %(name)s - %(message)s')
    import sys
    path = sys.argv[1] if len(sys.argv) > 1 else "hoge"
    test = PcmReader(path)
    test.read()





