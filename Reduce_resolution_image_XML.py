import os
import xml.etree.ElementTree as ET
from PIL import Image

def resize_image(image_path, target_size=(512, 512)):
    with Image.open(image_path) as img:
        resized_img = img.resize(target_size, Image.LANCZOS)
        resized_img.save(image_path)

def update_xml(xml_path, original_size, target_size=(512, 512)):
    tree = ET.parse(xml_path)
    root = tree.getroot()

    orig_width, orig_height = original_size
    new_width, new_height = target_size

    scale_x = new_width / orig_width
    scale_y = new_height / orig_height

    for size in root.findall('size'):
        size.find('width').text = str(new_width)
        size.find('height').text = str(new_height)

    for obj in root.findall('object'):
        bbox = obj.find('bndbox')
        xmin = bbox.find('xmin')
        ymin = bbox.find('ymin')
        xmax = bbox.find('xmax')
        ymax = bbox.find('ymax')

        xmin.text = str(int(int(xmin.text) * scale_x))
        ymin.text = str(int(int(ymin.text) * scale_y))
        xmax.text = str(int(int(xmax.text) * scale_x))
        ymax.text = str(int(int(ymax.text) * scale_y))

    tree.write(xml_path)

def process_folder(folder_path, target_size=(512, 512)):
    for filename in os.listdir(folder_path):
        if filename.endswith(".jpg") or filename.endswith(".png"):
            image_path = os.path.join(folder_path, filename)
            xml_path = os.path.join(folder_path, os.path.splitext(filename)[0] + ".xml")

            with Image.open(image_path) as img:
                original_size = img.size

            resize_image(image_path, target_size)
            update_xml(xml_path, original_size, target_size)

# Replace with the path to your folder
folder_path = r'D:\Solar Robot\Main_dataset\additional_images\Dust detection'
process_folder(folder_path, target_size=(512, 512))
