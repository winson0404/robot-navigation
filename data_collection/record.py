import cv2
import os


image_width = 640
image_height = 480
camera_index = 2
category = ["no_objects", "left", "middle", "right", "all" ,"left_mid", "right_mid"]


def validation_save_location(save_location):
    if not os.path.exists(save_location):
        print(f"path {save_location} does not exist, creating...")
        os.makedirs(save_location)
        return False
    
    return True

def get_counter():
    if validation_save_location("counter.txt"):
        with open("counter.txt", "r") as f:
            counter = int(f.read())
    else:
        counter = 0
        
    return counter

def update_counter(counter):
    with open("counter.txt", "w") as f:
        f.write(str(counter))
    return counter
        
def draw_rectangle(frame, x, y, w, h):
    x = int(x)
    y = int(y)
    w = int(w)
    h = int(h)
    cv2.rectangle(frame, (x,y), (x+w, y+h), (255,0,0), 2)
    return frame


def save_image(frame, path):
    # postprocess
    
    # resize image to half of the original size
    new_width = int(image_width/2)
    new_height = int(image_height/2)
    # breakpoint()
    frame = cv2.resize(frame, (new_width, new_height))
    
    # write counter
    cv2.imwrite(path, frame)
    print("Saved", path)
    counter = get_counter()
    with open("counter.txt", "w") as f:
        f.write(str(counter))
    counter+=1
    update_counter(counter)
        
def capture_postprocess(default_direction, save_root_path, pressed_key, frame, original_frame):
    cv2.imshow("Captured Frame", frame)
    pressed_key = cv2.waitKey(0) & 0xFF
    
    if (pressed_key == 32):
        counter = get_counter()
        save_path = os.path.join(save_root_path, default_direction, f"{default_direction}{counter}.png")
        save_image(original_frame, save_path)
        cv2.destroyWindow("Captured Frame")

        
    for i in range(len(category)):
        if (pressed_key == ord(str(i+1))):
            counter = get_counter()
            direction = category[i]
            validation_save_location(os.path.join(save_root_path, direction))
            save_path = os.path.join(save_root_path, direction, f"{direction}{counter}.png")
            save_image(original_frame, save_path)
            cv2.destroyWindow("Captured Frame")
        
    if (pressed_key == ord('e')):
        # cap.release()
        cv2.destroyWindow("Captured Frame")

if __name__ == "__main__":
    if validation_save_location("counter.txt"):
        with open("counter.txt", "r") as f:
            counter = int(f.read())
    else:
        counter = 0


    while(True):
        validation_save_location("output")
        # os.system("pause")
        table = input("Enter table: ")
        if table == "q":
            break
        table = os.path.join("output", table)
        validation_save_location(table)
        
        prompt = "Available directions:\n"
        for i in range(len(category)):
            prompt += f"{i+1}. {category[i]} \n"
        
        
        direction = category[int(input(prompt))-1]
        save_path = f"./{table}/{direction}/"
        validation_save_location(save_path)
        cap = cv2.VideoCapture(camera_index)
        while(True):
            ret, frame = cap.read()
            # write the bottom 20% of the frame
            original_frame = frame.copy()
            frame = draw_rectangle(frame, 0,  image_height*0.6, image_width, image_height*0.4)
            cv2.imshow(f"{table}_{direction}", frame)
            pressed_key = cv2.waitKey(1) & 0xFF
            
            if (pressed_key== ord('c')):
                capture_postprocess(default_direction=direction, save_root_path=table, pressed_key=pressed_key, frame=frame, original_frame=original_frame)
            
            if (pressed_key == ord('q')):
                cap.release()
                cv2.destroyAllWindows()
                break   