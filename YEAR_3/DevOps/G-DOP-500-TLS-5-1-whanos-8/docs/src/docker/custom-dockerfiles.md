# Custom Dockerfiles

How to use custom Dockerfiles with Whanos base images.

## Example

```dockerfile
FROM whanos-python:latest
RUN pip install custom-package
CMD ["python", "-m", "app"]
```
