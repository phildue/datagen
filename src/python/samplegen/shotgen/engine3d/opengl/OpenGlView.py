import numpy as np
from OpenGL.GL import glColor, GL_POLYGON, glBegin, glNormal3fv, glTexCoord2fv, glVertex3fv, glEnd, glDisable, \
    glEndList
from OpenGL.raw.GL.VERSION.GL_1_0 import glCallList
from OpenGL.raw.GL.VERSION.GL_1_0 import glGenLists, glNewList, glEnable
from OpenGL.raw.GL.VERSION.GL_1_1 import GL_COMPILE, glFrontFace, GL_CCW, GL_TEXTURE_2D, glBindTexture

from samplegen.scene.Gate import Gate
from samplegen.shotgen.engine3d.View import View
from samplegen.shotgen.engine3d.opengl.FileParser import FileParser
from utils.labels.Pose import Pose


class OpenGlView(View):
    def __init__(self, model: Gate, path: str, filename: str):
        self.model = model
        self.filename = filename
        self.path = path
        self.offset = Pose()
        self.faces, self.mtls, self.normals, self.texcoords, self.vertices = FileParser.parse_obj(path, filename)
        self.gl_list = None

    def __move(self, offset: Pose):

        vertices_mat = np.array([[v[0], v[1], v[2], 1] for v in self.vertices]).T
        transf_mat = offset.transfmat
        transf_mat[2] = -transf_mat[
            2]  # positive direction towards view, for some reason this is only required when moving an object
        vertices_moved = np.matmul(transf_mat, vertices_mat)
        return [(vertices_moved[0, i], vertices_moved[1, i], vertices_moved[2, i]) for i in range(len(self.vertices))]

    def draw(self, offset: Pose = Pose(0, 0, 0, 0, 0, 0)):
        if offset != self.offset or self.gl_list is None:
            vertices = self.__move(offset)
            self.offset = offset
            self.gl_list = self.__gen_gl_code(self.faces, self.mtls, self.normals, vertices, self.texcoords)

        glCallList(self.gl_list)

    @staticmethod
    def __gen_gl_code(faces, mtls, normals, vertices, texcoords):
        gl_list = glGenLists(1)
        glNewList(gl_list, GL_COMPILE)
        glEnable(GL_TEXTURE_2D)
        glFrontFace(GL_CCW)
        for face in faces:
            face_vertices, face_normals, texture_coords, material = face

            try:
                mtl = mtls[material]
                if 'texture_Kd' in mtl:
                    # use diffuse texmap
                    glBindTexture(GL_TEXTURE_2D, mtl['texture_Kd'])
                else:
                    # just use diffuse colour
                    glColor(mtl['Kd'])
            except KeyError:
                print("Key error" + str(material))
                glColor((1, 1, 1))

            glBegin(GL_POLYGON)
            for i in range(len(face_vertices)):
                if face_normals[i] > 0:
                    glNormal3fv(normals[face_normals[i] - 1])
                if texture_coords[i] > 0:
                    glTexCoord2fv(texcoords[texture_coords[i] - 1])
                glVertex3fv(vertices[face_vertices[i] - 1])
            glEnd()
        glDisable(GL_TEXTURE_2D)
        glEndList()
        return gl_list
