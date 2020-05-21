import redis
import json, os

def lambda_handler(event, context):
    r = redis.Redis(host=os.environ['REDIS_HOST'], port=6379, db=0)

    if event["detail"]["group"] == "service:" + os.environ["ECS_SERVICE"]:
        desired = event["detail"]["desiredStatus"]
        last = event["detail"]["lastStatus"]
        if desired == "RUNNING" and desired == last:
            print("Added task: " + event["detail"]["taskArn"])
            r.lpush("tasks", event["detail"]["taskArn"])
        elif desired == "STOPPED" or last == "STOPPED":
            r.lrem("tasks", event["detail"]["taskArn"], 1)
            print("Removed task: " + event["detail"]["taskArn"], 1)