defmodule SmoothieApi.Web.Schema.Member.Queries do
  use Absinthe.Schema.Notation

  object :member_query do
    ## ad authentication in here somehow
    field(:member, non_null(:user), resolve: &get_member/2)
  end

  
  def get_member(_args, info) do
    {:ok, info.context.current_user.id}
  end
end
