
ASSESSMENT_DB_URL=`heroku config:get DATABASE_URL -a tiro`
PROVIDER_DB_URL=`heroku config:get HEROKU_POSTGRESQL_GREEN_URL -a tiro`

# create-prod-db :
# 	export ASSESSMENT_DB_URL=${ASSESSMENT_DB_URL}; \
# 	export PROVIDER_DB_URL=${PROVIDER_DB_URL}; \
# 	cd api && \
# 	mix deps.get ; \
# 	MIX_ENV=dev mix ecto.migrate_seed

clean :
	rm -rf api/_build api/deps frontend/node_modules frontend/.cache frontend/lib/ frontend/build/ frontend/yarn-error.log api/.elixir_ls

up :
	export ASSESSMENT_DB_URL=${ASSESSMENT_DB_URL}; \
	export PROVIDER_DB_URL=${PROVIDER_DB_URL}; \
	kompose up

down :
	kompose down