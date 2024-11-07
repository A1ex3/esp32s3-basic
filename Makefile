IDF := idf.py
DEVICE := esp32s3
DOCKER_IMAGE := espressif/idf:latest
IDF_TARGET := $(DEVICE)

.PHONY: set-target
set-target:
	$(IDF) set-target $(DEVICE)

.PHONY: build
build:
	$(IDF) build

.PHONY: flash
flash:
	$(IDF) flash

PORT ?=
.PHONY: flash-port
flash-port:
	$(IDF) -p $(PORT) flash

.PHONY: fullclean
fullclean:
	$(IDF) fullclean

.PHONY: build_in_docker
build_in_docker:
	docker run --rm -v .:/project -w /project $(DOCKER_IMAGE) /bin/bash -c "make set-target && make build"