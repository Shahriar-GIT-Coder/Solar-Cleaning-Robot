import cv2
import os
import xml.etree.ElementTree as ET
import matplotlib.pyplot as plt


def get_annotations(xml_file):
    tree = ET.parse(xml_file)
    root = tree.getroot()
    bboxes = []

    for obj in root.findall('object'):
        bbox = obj.find('bndbox')
        xmin = int(bbox.find('xmin').text)
        ymin = int(bbox.find('ymin').text)
        xmax = int(bbox.find('xmax').text)
        ymax = int(bbox.find('ymax').text)
        bboxes.append((xmin, ymin, xmax, ymax))

    return bboxes


def draw_bboxes(image_path, bboxes):
    image = cv2.imread(image_path)
    for (xmin, ymin, xmax, ymax) in bboxes:
        cv2.rectangle(image, (xmin, ymin), (xmax, ymax), (255, 0, 0), 2)
    return image


def show_image(image):
    plt.figure(figsize=(10, 10))
    plt.imshow(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
    plt.axis('off')
    plt.show()


def main(image_dir, xml_dir):
    for xml_file in os.listdir(xml_dir):
        if xml_file.endswith('.xml'):
            xml_path = os.path.join(xml_dir, xml_file)
            image_name = xml_file.replace('.xml', '.jpg')  # assuming image extension is .jpg
            image_path = os.path.join(image_dir, image_name)

            if os.path.exists(image_path):
                bboxes = get_annotations(xml_path)
                image_with_bboxes = draw_bboxes(image_path, bboxes)
                show_image(image_with_bboxes)
            else:
                print(f"Image file {image_name} not found.")


# Update these paths
image_directory = r'D:\Solar Robot\Main_dataset\additional_images\New folder'
xml_directory = r'D:\Solar Robot\Main_dataset\additional_images\New folder'

main(image_directory, xml_directory)
