NAME = fhecourse
SHELL = bash
CONTAINER_ID = `docker ps | grep fhecourse | awk '{print $$1}'`

.PHONY: help

help:  ## List commands
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

image: ## Build the image
	docker build . -t fhecourse

start: ## Start the image (container)
	docker run -v $(PWD)/src:/opt/build/src -idt fhecourse

stop: ## Stop the container
	docker stop $(CONTAINER_ID)

bash: ## Login to the container
	@docker exec -it $(CONTAINER_ID) bash

run: ## Compile and run
	@docker exec -it $(CONTAINER_ID) bash -c "cmake ./src && make && ./fhecourse"