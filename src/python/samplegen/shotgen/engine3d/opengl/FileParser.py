import numpy as np
import pygame
from OpenGL.GL import glGenTextures, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_RGBA, GL_UNSIGNED_BYTE, \
    glTexImage2D, \
    glTexParameteri, GL_LINEAR
from OpenGL.raw.GL.VERSION.GL_1_1 import GL_TEXTURE_2D, glBindTexture


class FileParser:
    @staticmethod
    def parse_mtl(filename):
        contents = {}
        mtl = None
        for line in open(filename, "r"):
            if line.startswith('#'): continue
            values = line.split()
            if not values: continue
            if values[0] == 'newmtl':
                mtl = contents[values[1]] = {}
            elif mtl is None:
                raise ValueError("mtl file doesn't start with newmtl stmt")
            elif values[0] == 'map_Kd':

                # load the texture referred to by this declaration
                mtl[values[0]] = values[1]
                surf_file = mtl['map_Kd']
                if surf_file.endswith('.tga'):
                    TGA_Header
                surf = pygame.image.load()
                image = pygame.image.tostring(surf, 'RGBA', 1)
                ix, iy = surf.get_rect().size
                texid = mtl['texture_Kd'] = glGenTextures(1)
                glBindTexture(GL_TEXTURE_2D, texid)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ix, iy, 0, GL_RGBA,
                             GL_UNSIGNED_BYTE, image)
            else:
                mtl[values[0]] = list(map(float, values[1:]))
        return contents

    @staticmethod
    def parse_obj(path, filename, swapyz=False):
        """Loads a Wavefront OBJ file. """
        vertices = []
        normals = []
        texcoords = []
        faces = []
        mtls = None
        material = None
        for line in open(path + filename, "r"):
            if line.startswith('#'): continue
            values = line.split()
            if not values: continue
            if values[0] == 'v':
                v = list(map(float, values[1:4]))
                v = -v[0], -v[1], v[2]
                if swapyz:
                    v = v[0], v[2], v[1]
                vertices.append(v)
            elif values[0] == 'vn':
                v = list(map(float, values[1:4]))
                v = -v[0], -v[1], v[2]
                if swapyz:
                    v = v[0], v[2], v[1]
                normals.append(v)
            elif values[0] == 'vt':
                texcoords.append(map(float, values[1:3]))
            elif values[0] in ('usemtl', 'usemat'):
                material = values[1]
            elif values[0] == 'mtllib':
                mtls = FileParser.parse_mtl(path + values[1])
            elif values[0] == 'f':
                face = []
                texcoords = []
                norms = []
                for v in values[1:]:
                    w = v.split('/')
                    face.append(int(w[0]))
                    if len(w) >= 2 and len(w[1]) > 0:
                        texcoords.append(int(w[1]))
                    else:
                        texcoords.append(0)
                    if len(w) >= 3 and len(w[2]) > 0:
                        norms.append(int(w[2]))
                    else:
                        norms.append(0)
                faces.append((face, norms, texcoords, material))
        # vertices_scaled = np.array(vertices)*100
        # vertices = [(vertices_scaled[i, 0], vertices_scaled[i, 1], vertices_scaled[i, 2]) for i in range(len(vertices))]
        return faces, mtls, normals, texcoords, vertices
