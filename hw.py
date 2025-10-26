# import cv2
# import mediapipe as mp
 
# mp_drawing = mp.solutions.drawing_utils    
# mp_drawing_styles = mp.solutions.drawing_styles    
# mp_hands = mp.solutions.hands    
 
# cap = cv2.VideoCapture(0)
# cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)    
# cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)    
# cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)    
 
# cv2.namedWindow('MediaPipe Hands', cv2.WND_PROP_FULLSCREEN)
# cv2.setWindowProperty('MediaPipe Hands', cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
 
# with mp_hands.Hands(
#     model_complexity=0,
#     min_detection_confidence=0.5,
#     min_tracking_confidence=0.5) as hands:
#   while cap.isOpened():
#     success, image = cap.read()
#     if not success:
#         print("Ignoring empty camera frame.")
#         continue
 
#     image.flags.writeable = False
#     image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
 
#     results = hands.process(image)
 
#     image.flags.writeable = True
#     image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
 
#     if results.multi_hand_landmarks:
#         for hand_landmarks in results.multi_hand_landmarks:
#             mp_drawing.draw_landmarks(
#                 image,
#                 hand_landmarks,
#                 mp_hands.HAND_CONNECTIONS,
#                 mp_drawing_styles.get_default_hand_landmarks_style(),
#                 mp_drawing_styles.get_default_hand_connections_style())
 
#     cv2.imshow('MediaPipe Hands', cv2.flip(image, 1))
 
#     if cv2.waitKey(5) & 0xFF == 27:
#         break
 
# cap.release()


import cv2
import mediapipe as mp
import time
import requests


mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands

cap = cv2.VideoCapture(0)
url = "http://10.0.0.29:8080/updateDir"
payload = {"direction": 0}

def finger_up(hand_landmarks, finger_tip, finger_pip):
    return hand_landmarks.landmark[finger_tip].y < hand_landmarks.landmark[finger_pip].y

def process_images(hands):
    success, image = cap.read()

    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(image)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)

            # Example: Detect thumbs up
            thumb_up = finger_up(hand_landmarks, 4, 3)
            index_down = not finger_up(hand_landmarks, 8, 6)
            index_up = finger_up(hand_landmarks, 8, 6)
            middle_down = not finger_up(hand_landmarks, 12, 10)
            middle_up = finger_up(hand_landmarks, 12, 10)
            ring_down = not finger_up(hand_landmarks, 16, 14)
            pinky_down = not finger_up(hand_landmarks, 20, 18)
            

            if thumb_up and index_down and middle_down and ring_down and pinky_down:
                cv2.putText(image, "👍 Thumbs Up Detected!", (50, 50),
                            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                # 👉 Perform your action here
                #print("Action: Thumbs Up!")
                return 1
            
            elif thumb_up and index_up and middle_up and ring_down and pinky_down:
                cv2.putText(image, "Shoot!", (50, 50),
                            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                # 👉 Perform your action here
                #print("Action: Thumbs Up!")
                return -1
            else:
                return 0

    cv2.imshow('MediaPipe Hands', cv2.flip(image, 1))
    if cv2.waitKey(5) & 0xFF == 27:
        return 0

def exit_processing():
    global cap
    cap.release()
    cv2.destroyAllWindows()


with mp_hands.Hands(min_detection_confidence=0.7, min_tracking_confidence=0.7) as hands:
        while cap.isOpened():
            time.sleep(1)
            output = process_images(hands)
            payload["direction"] = output
            response = requests.post(url, json=payload)

