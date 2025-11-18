import cv2
import mediapipe as mp
import numpy as np

# Initialize MediaPipe
mp_face_mesh = mp.solutions.face_mesh
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

# Initialize camera
cap = cv2.VideoCapture(0)

# Initialize detection models
face_mesh = mp_face_mesh.FaceMesh(
    max_num_faces=1,
    refine_landmarks=True,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5,
)
hands = mp_hands.Hands(max_num_hands=2, min_detection_confidence=0.7, min_tracking_confidence=0.5)


def detect_smile(face_landmarks):
    """Detect if person is smiling based on mouth corners"""
    if not face_landmarks:
        return False

    # Get mouth corner landmarks (left: 61, right: 291)
    # Get upper lip center: 13, lower lip center: 14
    landmarks = face_landmarks.landmark

    left_mouth = landmarks[61]
    right_mouth = landmarks[291]
    upper_lip = landmarks[13]
    lower_lip = landmarks[14]

    # Calculate mouth width and height
    mouth_width = abs(right_mouth.x - left_mouth.x)
    mouth_height = abs(lower_lip.y - upper_lip.y)

    # Smile detection: wider mouth relative to height
    smile_ratio = mouth_width / (mouth_height + 0.001)

    return smile_ratio > 5


def detect_hands_up(hand_landmarks, frame_height):
    """Detect if hands are raised above head level"""
    if not hand_landmarks:
        return False

    hands_up = False
    for hand_landmark in hand_landmarks:
        # Get wrist position (landmark 0)
        wrist_y = hand_landmark.landmark[0].y

        # If wrist is in upper third of frame, hands are up
        if wrist_y < 0.4:
            hands_up = True
            break

    return hands_up


# Create placeholder images for different emotions
def create_image_with_text(text, color):
    """Create a colored image with text"""
    img = np.zeros((400, 600, 3), dtype=np.uint8)
    img[:] = color

    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 2
    thickness = 3

    # Get text size to center it
    text_size = cv2.getTextSize(text, font, font_scale, thickness)[0]
    text_x = (img.shape[1] - text_size[0]) // 2
    text_y = (img.shape[0] + text_size[1]) // 2

    cv2.putText(img, text, (text_x, text_y), font, font_scale, (255, 255, 255), thickness)

    return img


# Create emotion images
neutral_img = create_image_with_text("NEUTRAL", (100, 100, 100))
smile_img = create_image_with_text("SMILING! :)", (50, 200, 50))
hands_up_img = create_image_with_text("HANDS UP!", (200, 100, 50))

print("Camera started! Press 'q' to quit")
print("Try smiling or raising your hands!")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Flip frame horizontally for mirror effect
    frame = cv2.flip(frame, 1)

    # Convert to RGB for MediaPipe
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # Process face and hands
    face_results = face_mesh.process(rgb_frame)
    hand_results = hands.process(rgb_frame)

    # Detect emotions/gestures
    is_smiling = False
    are_hands_up = False

    if face_results.multi_face_landmarks:
        is_smiling = detect_smile(face_results.multi_face_landmarks[0])

        # Draw face mesh
        for face_landmarks in face_results.multi_face_landmarks:
            mp_drawing.draw_landmarks(
                frame,
                face_landmarks,
                mp_face_mesh.FACEMESH_CONTOURS,
                landmark_drawing_spec=mp_drawing.DrawingSpec(
                    color=(0, 255, 0), thickness=1, circle_radius=1
                ),
            )

    if hand_results.multi_hand_landmarks:
        are_hands_up = detect_hands_up(hand_results.multi_hand_landmarks, frame.shape[0])

        # Draw hand landmarks
        for hand_landmarks in hand_results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)

    # Choose which image to display
    if are_hands_up:
        display_img = hands_up_img
        status = "HANDS UP!"
    elif is_smiling:
        display_img = smile_img
        status = "SMILING"
    else:
        display_img = neutral_img
        status = "NEUTRAL"

    # Add status text to camera frame
    cv2.putText(frame, f"Status: {status}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    # Display windows
    cv2.imshow("Camera Feed", frame)
    cv2.imshow("Emotion Display", display_img)

    # Press 'q' to quit
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

# Cleanup
cap.release()
cv2.destroyAllWindows()
