import os
from PIL import Image

# Directory containing images
image_dir = 'D:\Solar Robot\image'  # Replace with the actual path to your image directory
new_resolution = (320, 320)  # Desired resolution for TensorFlow Lite model training


# Function to resize images
def resize_images(directory, new_resolution):
    image_files = sorted([f for f in os.listdir(directory) if f.endswith(('.jpg', '.jpeg', '.png'))])

    for image_file in image_files:
        image_path = os.path.join(directory, image_file)

        with Image.open(image_path) as img:
            # Resize the image to the new resolution
            img = img.resize(new_resolution, Image.LANCZOS)
            img.save(image_path)
            print(f'Resized {image_path} to {img.size}')


# Run the function to resize images
resize_images(image_dir, new_resolution)
