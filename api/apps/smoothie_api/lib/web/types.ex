defmodule SmoothieApi.Web.Types do
  use Absinthe.Schema.Notation

  import_types(SmoothieApi.Web.Schema.Domain.AppIngredient.Query)
  import_types(SmoothieApi.Web.Schema.Domain.DraftRecipe.Query)
  import_types(SmoothieApi.Web.Schema.Domain.Ingredient.Query)
  import_types(SmoothieApi.Web.Schema.Domain.Macro.Query)
  import_types(SmoothieApi.Web.Schema.Domain.MacroInfo.Query)
  import_types(SmoothieApi.Web.Schema.Domain.NBDBrandedIngredient.Query)
  import_types(SmoothieApi.Web.Schema.Domain.NBDIngredient.Query)
  import_types(SmoothieApi.Web.Schema.Domain.Nutrient.Query)
  import_types(SmoothieApi.Web.Schema.Domain.NutrientInfo.Query)
  import_types(SmoothieApi.Web.Schema.Domain.NutritionalInfo.Query)
  import_types(SmoothieApi.Web.Schema.Domain.Profile.Query)
  import_types(SmoothieApi.Web.Schema.Domain.Recipe.Query)
  import_types(SmoothieApi.Web.Schema.Domain.User.Query)
  import_types(SmoothieApi.Web.Schema.Domain.UserRecipe.Query)
end
