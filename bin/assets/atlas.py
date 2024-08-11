import os
import math
from PIL import Image

def cria_atlas_line(pasta, atlas_nome):
    # Procurar imagens PNG e JPEG
    arquivos = [f for f in os.listdir(pasta) if f.lower().endswith(('.png', '.jpg', '.jpeg'))]
    imagens = [Image.open(os.path.join(pasta, f)) for f in arquivos]

    # Calcule as dimensões do atlas
    largura_atlas = max([img.width for img in imagens]) * len(imagens)
    altura_atlas = max([img.height for img in imagens])

    # Crie o atlas
    atlas = Image.new('RGBA', (largura_atlas, altura_atlas), (0, 0, 0, 0))

    # Coloque as imagens em grid
    for idx, img in enumerate(imagens):
        x = idx * img.width
        y = 0
        atlas.paste(img, (x, y))

    # Salve o atlas
    atlas.save(atlas_nome)

def cria_atlas_space(pasta, atlas_nome, espaco=0, margem=0):
    # Procurar imagens PNG e JPEG
    arquivos = [f for f in os.listdir(pasta) if f.lower().endswith(('.png', '.jpg', '.jpeg'))]
    imagens = [Image.open(os.path.join(pasta, f)) for f in arquivos]

    # Calcule as dimensões do atlas
    largura_atlas = (max([img.width for img in imagens]) + espaco) * len(imagens) - espaco + 2 * margem
    altura_atlas = max([img.height for img in imagens]) + 2 * margem

    # Crie o atlas
    atlas = Image.new('RGBA', (largura_atlas, altura_atlas), (0, 0, 0, 0))

    # Coloque as imagens em grid
    for idx, img in enumerate(imagens):
        x = margem + idx * (img.width + espaco)
        y = margem
        atlas.paste(img, (x, y))

    # Salve o atlas
    atlas.save(atlas_nome)
    
def cria_atlas(pasta, atlas_nome, espaco=0, margem=0, colunas=None):
    # Procurar imagens PNG e JPEG
    arquivos = [f for f in os.listdir(pasta) if f.lower().endswith(('.png', '.jpg', '.jpeg'))]
    imagens = [Image.open(os.path.join(pasta, f)) for f in arquivos]

    # Calcular o número de colunas automaticamente se não fornecido
    if colunas is None:
        colunas = math.ceil(math.sqrt(len(imagens)))

    # Calcule o número de linhas necessárias
    linhas = math.ceil(len(imagens) / colunas)

    # Calcule as dimensões do atlas
    largura_atlas = (max([img.width for img in imagens]) + espaco) * colunas - espaco + 2 * margem
    altura_atlas = (max([img.height for img in imagens]) + espaco) * linhas - espaco + 2 * margem

    # Crie o atlas
    atlas = Image.new('RGBA', (largura_atlas, altura_atlas), (0, 0, 0, 0))

    # Coloque as imagens em grid
    for idx, img in enumerate(imagens):
        coluna = idx % colunas
        linha = idx // colunas
        x = margem + coluna * (img.width + espaco)
        y = margem + linha * (img.height + espaco)
        atlas.paste(img, (x, y))

    # Salve o atlas
    atlas.save(atlas_nome)

    
# Exemplo de uso
pasta = './tiles/'
atlas_nome = 'atlas.png'
cria_atlas(pasta, atlas_nome)
