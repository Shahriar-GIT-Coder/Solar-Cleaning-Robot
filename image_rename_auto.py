import os

# Directory containing images
image_dir = 'D:\Solar Robot\Main_dataset\images'  # Replace with the actual path to your image directory


# Function to rename images
def rename_images(directory):
    image_files = sorted([f for f in os.listdir(directory) if f.endswith('.jpg')])

    for idx, image_file in enumerate(image_files, start=1):
        new_name = f'img{idx}.jpg'
        old_path = os.path.join(directory, image_file)
        new_path = os.path.join(directory, new_name)

        os.rename(old_path, new_path)
        print(f'Renamed {old_path} to {new_path}')


# Run the function to rename images
rename_images(image_dir)
