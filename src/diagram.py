import sys
import json
import numpy as np
import matplotlib.pyplot as plt


class FrameDiagram:

    def __init__(self, dir):
        with open(dir + '/reaction.json', 'r') as infile:
            self.data = json.load(infile)
        self.sfd_path = dir + '/sfd.png'
        self.bmd_path = dir + '/bmd.png'
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


    def compute_element_sfd(self, element_json):
        self.diagram['sfd'] = self.diagram['element'].copy()
        self.diagram['sfd'][1, :] -= self.diagram['element'][1, 0]
        self.diagram['sfd'][1, :] += element_json['reaction'][1]


    def compute_element_bmd(self, element_json):
        self.diagram['bmd'] = self.diagram['element'].copy()
        self.diagram['bmd'][1, :] -= self.diagram['element'][1, 0]
        self.diagram['bmd'][1, :] += element_json['reaction'][2]

        sfd_x = self.diagram['sfd'][0, :]
        sfd_y = self.diagram['sfd'][1, :]
        for ix in range(1, len(sfd_x)):
            dx = sfd_x[ix] - sfd_x[ix - 1]
            fx = sfd_y[ix]
            self.diagram['bmd'][1, ix:] -= fx * dx


    def adjust_element_diagram(self, element_json, scale):
        # adjust scale
        self.diagram['sfd'][1, :] *= scale[0]
        self.diagram['bmd'][1, :] *= scale[1]

        # put sfd & bmd on the element
        self.diagram['sfd'][1, :] += self.diagram['element'][1, 0]
        self.diagram['bmd'][1, :] += self.diagram['element'][1, 0]

        # convert to global coordinate system
        rot = self.rotation_matrix(element_json)
        self.diagram['element'] = rot.dot(self.diagram['element'])
        self.diagram['sfd'] = rot.dot(self.diagram['sfd'])
        self.diagram['bmd'] = rot.dot(self.diagram['bmd'])

        # connect endpoint of sfd & bmd with element
        left = self.diagram['element'][:, 0].reshape(2, 1)
        right = self.diagram['element'][:, -1].reshape(2, 1)
        self.diagram['sfd'] = np.concatenate((left, self.diagram['sfd'], right), axis=1)
        self.diagram['bmd'] = np.concatenate((left, self.diagram['bmd'], right), axis=1)


    def draw_diagram(self, scale=(1.0, 1.0)):
        sfd_fig = plt.figure(0)
        bmd_fig = plt.figure(1)
        sfd_plot = sfd_fig.add_subplot(1, 1, 1, aspect = 'equal')
        bmd_plot = bmd_fig.add_subplot(1, 1, 1, aspect = 'equal')

        for ix, elm_jsn in enumerate(self.data['element']):
            self.set_element_coord(elm_jsn)
            self.compute_element_sfd(elm_jsn)
            self.compute_element_bmd(elm_jsn)
            self.adjust_element_diagram(elm_jsn, scale)

            # plot
            sfd_plot.plot(self.diagram['element'][0, :], self.diagram['element'][1, :], 'b-')
            sfd_plot.plot(self.diagram['sfd'][0, :], self.diagram['sfd'][1, :], 'r--')
            bmd_plot.plot(self.diagram['element'][0, :], self.diagram['element'][1, :], 'b-')
            bmd_plot.plot(self.diagram['bmd'][0, :], self.diagram['bmd'][1, :], 'r--')

        sfd_fig.savefig(self.sfd_path)
        bmd_fig.savefig(self.bmd_path)


def main():
    fd = FrameDiagram(dir=sys.argv[1])
    fd.draw_diagram(scale=(5.0, 0.5))


main()
