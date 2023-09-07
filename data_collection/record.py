import cv2
import os


category = ["front", "left", "right"]

def validation_save_location(save_location):
    if not os.path.exists(save_location):
        print(f"path {save_location} does not exist, creating...")
        os.makedirs(save_location)
        
def capture_postprocess(counter, default_direction, save_root_path, pressed_key, frame):
    cv2.imshow("Captured Frame", frame)
    print("Captured", counter)
    pressed_key = cv2.waitKey(0) & 0xFF
    
    if (pressed_key == 32):
        save_path = os.path.join(save_root_path, default_direction, f"{default_direction}{counter}.png")
        cv2.imwrite(save_path, frame)
        print("Saved", save_path)
        cv2.destroyWindow("Captured Frame")
        
    if (pressed_key == ord('1')):
        direction = category[0]
        save_path = os.path.join(save_root_path, direction, f"{direction}{counter}.png")
        cv2.imwrite(save_path, frame)
        print("Saved", save_path)
        cv2.destroyWindow("Captured Frame")
    if (pressed_key == ord('2')):
        direction = category[1]
        save_path = os.path.join(save_root_path, direction, f"{direction}{counter}.png")
        cv2.imwrite(save_path, frame)
        print("Saved", save_path)
        cv2.destroyWindow("Captured Frame")
    if (pressed_key == ord('3')):
        direction = category[2]
        save_path = os.path.join(save_root_path, direction, f"{direction}{counter}.png")
        cv2.imwrite(save_path, frame)
        print("Saved", save_path)
        cv2.destroyWindow("Captured Frame")
        
    
    if (pressed_key == ord('e')):
        # cap.release()
        cv2.destroyWindow("Captured Frame")


if __name__ == "__main__":
    while(True):
        validation_save_location("output")
        # os.system("pause")
        table = input("Enter table: ")
        table = os.path.join("output", table)
        if table == "q":
            break
        validation_save_location(table)

        direction = input("Enter direction: ")
        save_path = f"./{table}/{direction}/"
        validation_save_location(save_path)
        cap = cv2.VideoCapture(0)
        counter=0
        while(True):
            ret, frame = cap.read()
            cv2.imshow(f"{table}_{direction}", frame)
            pressed_key = cv2.waitKey(1) & 0xFF
            
            if (pressed_key== ord('c')):
                capture_postprocess(counter, default_direction=direction, save_root_path=table, pressed_key=pressed_key, frame=frame)
                counter+=1
            
            if (pressed_key == ord('q')):
                cap.release()
                cv2.destroyAllWindows()
                break