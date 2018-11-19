APP_NAME=fhe_course
APP_PATH=app/
.PHONY: help

help:
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

# DOCKER TASKS
# Build the container
build: ## Build the container
	docker build -t $(APP_NAME) .

build-nc: ## Build the container without caching
	docker build --no-cache -t $(APP_NAME) .

login: ## Login to the machine
	docker run -it $(APP_NAME) /bin/bash

app/%: build
	@echo Running $@ -> python3 $(notdir $@)
	docker run -it $(APP_NAME) python3 $(notdir $@)

#app/%: build ## run specific files
#	docker run -it python3 $@
