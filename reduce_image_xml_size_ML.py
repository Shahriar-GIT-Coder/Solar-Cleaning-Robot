
import os
from PIL import Image
import xml.etree

# Directory containing images and annotations
dataset_dir = 'D:\Solar Robot\Main_dataset\images'  # Replace with the actual path to your dataset directory
quality = 70  # Adjust the quality (1-100) to reduce file size

# Function to reduce image file size without changing resolution
def reduce_image_size_and_update_xml(directory, quality):
    image_files = sorted([f for f in os.listdir(directory) if f.endswith('.jpg')])
    annotation_files = sorted([f for f in os.listdir(directory) if f.endswith('.xml')])

    if len(image_files) != len(annotation_files):
        raise ValueError("The number of images and annotations do not match!")

    for image_file, annotation_file in zip(image_files, annotation_files):
        image_path = os.path.join(directory, image_file)
        annotation_path = os.path.join(directory, annotation_file)

        # Reduce the file size by lowering the image quality
        with Image.open(image_path) as img:
            img.save(image_path, "JPEG", quality=quality)

        # Update the XML file if needed (e.g., to reflect changes in file path or name)
        # In this case, no changes needed to XML content

        print(f'Reduced file size for: {image_path}')
        print(f'No change to XML: {annotation_path}')

# Run the function to reduce image file size
reduce_image_size_and_update_xml(dataset_dir, quality)
