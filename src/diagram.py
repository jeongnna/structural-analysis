import sys
import json
import numpy as np
import matplotlib.pyplot as plt


class FrameDiagram:

    def __init__(self, indatafile, outdatafile, outfigfile):
        with open(indatafile, 'r') as infile:
            self.indata = json.load(infile)
        self.outdatafile = outdatafile
        self.outfigfile = outfigfile
        self.diagram = {}


    def rotation_matrix(self, element_json):
        dx = element_json['node2_x'] - element_json['node1_x']
        dy = element_json['node2_y'] - element_json['node1_y']
        length = np.sqrt(dx**2 + dy**2)
        lx = dx / length
        ly = dy / length
        return np.array([[lx, -ly], [ly, lx]])


    def set_element_coord(self, element_json):
        elm_x = np.linspace(element_json['node1_x'], element_json['node2_x'], num=100).reshape(1, -1)
        elm_y = np.linspace(element_json['node1_y'], element_json['node2_y'], num=100).reshape(1, -1)
        self.diagram['element'] = np.concatenate((elm_x, elm_y), axis=0)

        # convert to local coordinate system
        rot = self.rotation_matrix(element_json)
        self.diagram['element'] = rot.T.dot(self.diagram['element'])


    def set_element_sfd(self, element_json):
        self.diagram['sfd'] = self.diagram['element'].copy()
        self.diagram['sfd'][1, :] -= self.diagram['element'][1, 0]
        self.diagram['sfd'][1, :] += element_json['reaction'][1]
        self.diagram['sfd'][1, -1] += element_json['reaction'][4]


    def set_element_bmd(self, element_json):
        self.diagram['bmd'] = self.diagram['element'].copy()
        self.diagram['bmd'][1, :] -= self.diagram['element'][1, 0]
        self.diagram['bmd'][1, :] += element_json['reaction'][2]
        self.diagram['bmd'][1, -1] += element_json['reaction'][5]

        sfd_x = self.diagram['sfd'][0, :]
        sfd_y = self.diagram['sfd'][1, :]
        for ix in range(1, len(sfd_x)):
            dx = sfd_x[ix] - sfd_x[ix - 1]
            fx = sfd_y[ix]
            self.diagram['bmd'][1, ix:] -= fx * dx


    def draw_diagram(self):
        outdata = open(self.outdatafile, 'w')

        fig, (sfd, bmd) = plt.subplots(1, 2)

        for ix, elm_jsn in enumerate(self.indata['element']):
            self.set_element_coord(elm_jsn)
            self.set_element_sfd(elm_jsn)
            self.set_element_bmd(elm_jsn)

            outdata.write('<Element %d>' % (ix + 1))
            outdata.write('\nx: ')
            outdata.write(np.array_str(self.diagram['element'][0, :]))
            outdata.write('\nsfd: ')
            outdata.write(np.array_str(self.diagram['sfd'][1, :]))
            outdata.write('\nbmd: ')
            outdata.write(np.array_str(self.diagram['bmd'][1, :]))
            outdata.write('\n\n')

            self.diagram['sfd'][1, :] += self.diagram['element'][1, 0]
            self.diagram['bmd'][1, :] += self.diagram['element'][1, 0]

            # convert to global coordinate system
            rot = self.rotation_matrix(elm_jsn)
            self.diagram['element'] = rot.dot(self.diagram['element'])
            self.diagram['sfd'] = rot.dot(self.diagram['sfd'])
            self.diagram['bmd'] = rot.dot(self.diagram['bmd'])
            # if not ix == 2:
            #     continue
            # plot
            sfd.plot(self.diagram['element'][0, :], self.diagram['element'][1, :], 'b-')
            sfd.plot(self.diagram['sfd'][0, :], self.diagram['sfd'][1, :], 'r-')
            bmd.plot(self.diagram['element'][0, :], self.diagram['element'][1, :], 'b-')
            bmd.plot(self.diagram['bmd'][0, :], self.diagram['bmd'][1, :], 'r-')
            # if ix == 0:
            #     break

        plt.savefig(self.outfigfile)
        outdata.close()


def main():
    sd = FrameDiagram(
        indatafile=sys.argv[1],
        outdatafile=sys.argv[2],
        outfigfile=sys.argv[3]
    )
    sd.draw_diagram()


main()
