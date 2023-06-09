import cv2
import datetime
import imutils
import numpy as np
from centroidtracker import CentroidTracker
from collections import defaultdict
from itertools import zip_longest
import requests

protopath = "MobileNetSSD_deploy.prototxt"
modelpath = "MobileNetSSD_deploy.caffemodel"
detector = cv2.dnn.readNetFromCaffe(prototxt=protopath, caffeModel=modelpath)


CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat",
           "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
           "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
           "sofa", "train", "tvmonitor"]

tracker = CentroidTracker(maxDisappeared=80, maxDistance=90)


def non_max_suppression_fast(boxes, overlapThresh):
    try:
        if len(boxes) == 0:
            return []

        if boxes.dtype.kind == "i":
            boxes = boxes.astype("float")

        pick = []

        x1 = boxes[:, 0]
        y1 = boxes[:, 1]
        x2 = boxes[:, 2]
        y2 = boxes[:, 3]

        area = (x2 - x1 + 1) * (y2 - y1 + 1)
        idxs = np.argsort(y2)

        while len(idxs) > 0:
            last = len(idxs) - 1
            i = idxs[last]
            pick.append(i)

            xx1 = np.maximum(x1[i], x1[idxs[:last]])
            yy1 = np.maximum(y1[i], y1[idxs[:last]])
            xx2 = np.minimum(x2[i], x2[idxs[:last]])
            yy2 = np.minimum(y2[i], y2[idxs[:last]])

            w = np.maximum(0, xx2 - xx1 + 1)
            h = np.maximum(0, yy2 - yy1 + 1)

            overlap = (w * h) / area[idxs[:last]]

            idxs = np.delete(idxs, np.concatenate(([last],
                                                   np.where(overlap > overlapThresh)[0])))

        return boxes[pick].astype("int")
    except Exception as e:
        print("Exception occurred in non_max_suppression : {}".format(e))


def main():
    # cap = cv2.VideoCapture('example_01.mp4')
    cap = cv2.VideoCapture(0)
    fps_start_time = datetime.datetime.now()
    fps = 0
    total_frames = 0
    lpc_count = 0
    total_count = 0
    centroid_dict = defaultdict(list)

    object_id_list = []

    while True:
        ret, frame = cap.read()
        frame = imutils.resize(frame, width=600)
        total_frames = total_frames + 1

        (H, W) = frame.shape[:2]

        blob = cv2.dnn.blobFromImage(frame, 0.007843, (W, H), 127.5)

        detector.setInput(blob)
        person_detections = detector.forward()
        rects = []
        for i in np.arange(0, person_detections.shape[2]):
            confidence = person_detections[0, 0, i, 2]
            if confidence > 0.5:
                idx = int(person_detections[0, 0, i, 1])

                if CLASSES[idx] != "person":
                    continue

                person_box = person_detections[0, 0, i, 3:7] * np.array([W, H, W, H])
                (startX, startY, endX, endY) = person_box.astype("int")
                rects.append(person_box)

        boundingboxes = np.array(rects)
        boundingboxes = boundingboxes.astype(int)
        rects = non_max_suppression_fast(boundingboxes, 0.3)

        objects = tracker.update(rects)
        # cv2.line(frame, ( 0 ,H //2), (W , H //2 ), (0, 0, 255), 2)


        for (objectId, bbox) in objects.items():
            x1, y1, x2, y2 = bbox
            x1 = int(x1)
            y1 = int(y1)
            x2 = int(x2)
            y2 = int(y2)

            cX = int((x1 + x2) / 2.0)
            cY = int((y1 + y2) / 2.0)
            cv2.circle(frame, (cX, cY), 4, (0, 255, 0), -1)

            centroid_dict[objectId].append((cX, cY))
            if objectId not in object_id_list:
                object_id_list.append(objectId)
                start_pt = (cX, cY)
                end_pt = (cX, cY)
                # cv2.line(frame, start_pt, end_pt, (0, 255, 0), 2)
            else:
                l = len(centroid_dict[objectId])
                for pt in range(len(centroid_dict[objectId])):
                    if not pt + 1 == l:
                        start_pt = (centroid_dict[objectId][pt][0], centroid_dict[objectId][pt][1])
                        end_pt = (centroid_dict[objectId][pt + 1][0], centroid_dict[objectId][pt + 1][1])
                        # cv2.line(frame, start_pt, end_pt, (0, 255, 0), 2)

            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 2)
            text = "ID: {}".format(objectId)
            cv2.putText(frame, text, (x1, y1 - 5), cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (0, 0, 255), 1)
            # cx=int(x1+x2)/2.0
            # cy=int(y1+y2)/2.0
            # cv2.circle(frame, (cx, cy), 4, (0, 0, 255), -1)
            # centroid_dict[objectId].append((cx, cy))
            # if objectId not in object_id_list:
            #     object_id_list.append(objectId)
            # cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 2)
            # text = "ID: {}".format(objectId)
            # cv2.putText(frame, text, (x1, y1-5), cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (0, 0, 255), 1)
            #
            # if objectId not in object_id_list:
            #      object_id_list.append(objectId)

        fps_end_time = datetime.datetime.now()
        time_diff = fps_end_time - fps_start_time
        if time_diff.seconds == 0:
            fps = 0.0
        else:
            fps = (total_frames / time_diff.seconds)

        fps_text = "FPS: {:.2f}".format(fps)

        cv2.putText(frame, fps_text, (5, 30), cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (0, 0, 255), 1)

        lpc_count = len(objects)
        total_count = len(object_id_list)

        lpc_txt = "LPC: {}".format(lpc_count)
        opc_txt = "Total: {}".format(total_count)

        cv2.putText(frame, lpc_txt, (5, 60), cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (0, 0, 255), 1)
        cv2.putText(frame, opc_txt, (5, 90), cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (0, 0, 255), 1)

        
        f=f'http://192.168.102.30/update_Info?LiveCount={lpc_count}'
        r=requests.get(f)
    

        cv2.imshow("Frame", frame)
        key = cv2.waitKey(1)
        if key == ord('q'):
            r=requests.get('http://192.168.102.30/update_Info?LiveCount=0')
            break

    cv2.destroyAllWindows()


main()