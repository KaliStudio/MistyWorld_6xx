//$Id: Abstract_Timer_Queue.h 95368 2011-12-19 13:38:49Z mcorino $

#ifndef ACE_ABSTRACT_TIMER_QUEUE_H
#define ACE_ABSTRACT_TIMER_QUEUE_H

#include /**/ "ace/pre.h"


ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// avant déclarée
class ACE_Time_Value;
class ACE_Command_Base;
template<typename TYPE> class ACE_Timer_Queue_Iterator_T;
template<typename TYPE> class ACE_Timer_Node_T;


template<typename TYPE>
class ACE_Abstract_Timer_Queue
{
public:
  /// Destructeur
  virtual ~ACE_Abstract_Timer_Queue (void) = 0;

  /// True si la file d'attente est vide, sinon false.
  virtual bool is_empty (void) const = 0;

  /// Renvoie l'heure du nœud plus tôt dans la file d'attente minuterie. il faut
  /// être appelé sur une file d'attente non vide.
  virtual const ACE_Time_Value &earliest_time (void) const = 0;

  /**
   * Le calendrier est @a un type qui expirera à @a future_time, qui est
   * spécifiée en temps absolu. S'il expire alors @a une loi est adoptée
   * en tant que valeur à <functor>.  Si @a un intervalle est != à
   * ACE_Time_Value::zero alors il est utilisé pour reporter @a type
   * automatiquement, en utilisant le temps par rapport au courant <gettimeofday>.
   * Cette méthode renvoie une <timer_id> qui identifie de façon unique le
   * @a type entrée dans une liste interne.  Ce <timer_id> peut être utilisé pour
   * annuler la minuterie avant son expiration. L'annulation assure
   * le <timer_ids> sont uniques jusqu'à des valeurs supérieures à 2
   * milliard de minuteries.  Tant que des minuteries ne restent pas plus longtemps que
   * cela, il devrait y avoir aucun problème avec la suppression accidentelle de la
   * minuterie erroné.  Retourne -1 en cas d'échec (qui est garanti de ne jamais
   * être valable <timer_id>).
   */
  virtual long schedule (const TYPE &type,
                         const void *act,
                         const ACE_Time_Value &future_time,
                         const ACE_Time_Value &interval = ACE_Time_Value::zero) = 0;

  /**
   * Exécutez le <functor> pour tous les compteurs dont les valeurs sont <= @a current_time.
   * Cela ne tient pas compte de <timer_skew>. Retourne le nombre de
   * minuteries annulés.
   */
  virtual int expire (const ACE_Time_Value &current_time) = 0;

  /**
   * Exécutez le <functor> pour tous les compteurs dont les valeurs sont <=
   * <ACE_OS::gettimeofday>.  Représente également <timer_skew>.
   *
   * Selon la résolution de l'OS sous-jacent les appels système
   * comme  select()/poll() pourrait revenir au heure différente de qui est
   * spécifiée dans le délai. Supposons que le système d'exploitation garantit une résolution de t ms.
   * Le calendrier va ressembler
   *
   *             A                   B
   *             |                   |
   *             V                   V
   *  |-------------|-------------|-------------|-------------|
   *  t             t             t             t             t
   *
   *
   * Si vous spécifiez une valeur de délai d'attente de A, le délai d'attente ne se produira pas
   * à A, mais à l'intervalle suivant de la minuterie, qui est plus tard que
   * que l'on attend. De même, si votre valeur d'expiration est égal à B,
   * alors le délai aura lieu à intervalle après B. Maintenant en fonction de la
   * résolution de vos délais d'attente et la précision des délais d'attente
   * nécessaire pour votre application, vous devez définir la valeur de
   * <timer_skew>. Dans le cas ci-dessus, si vous voulez que le délai d'attente de A à feu
   * au plus tard A, alors vous devriez spécifier votre <timer_skew> comme
   * A % t.
   *
   * La valeur de temporisation doit être spécifiée par la macro ACE_TIMER_SKEW
   * dans votre fichier config.h. La valeur par défaut est zéro.
   *
   * Les choses deviennent intéressantes si le t avant que la valeur du délai d'attente B soit à zéro
   * Soit votre délai est inférieur à l'intervalle. Dans ce cas, vous êtes
   * presque sûr de ne pas avoir le comportement d'attente souhaité. peut-être que vous
   * devrait trouver un meilleur système d'exploitation :-)
   *
   *  Retourne le nombre de minuteries annulé.
   */
  virtual int expire (void) = 0;

  /**
   * Un couple de classes en utilisant Timer_Queues a besoin d'envoyer un seul
   * événement à la fois. Mais avant de distribuer l'événement dont ils ont besoin pour
   * relâche un verrou, signaler d'autres fils, etc
   *
   * Cette fonction de membre doit être utilisé dans ce cas. Les opérations supplémentaires
   * doivent être appelés juste avant l'envoi de l'événement, et
   * seulement si un événement est envoyé, et encapsulé dans l'objet
   * ACE_Command_Base.
   */
  virtual int expire_single(ACE_Command_Base & pre_dispatch_command) = 0;

  /**
   * Remet l'intervalle de la minuterie représenté par @a timer_id à 
   * @a intervalle, qui est spécifié dans le temps par rapport au courant 
   * <gettimeofday>.  Si @a l'intervalle est égal à 
   * ACE_Time_Value::zero, la minuterie sera une minuterie non rééchelonnement.
   * Renvoie 0 en cas de succès, -1 sinon.
   */
  virtual int reset_interval (long timer_id,
                              const ACE_Time_Value &interval) = 0;

  /**
   * Annuler tous minuterie associée à un type @a. Si
   * @a dont_call_handle_close est 0, alors le <functor> sera invoqué,
   * qui invoque généralement le crochet de <handle_close>. Retourne le nombre
   * de minuteries annulés.
   */
  virtual int cancel (const TYPE &type,
                      int dont_call_handle_close = 1) = 0;

  /**
  * Annuler la minuterie unique qui correspond à la valeur de @a timer_id (qui
  * a été renvoyée par la méthode de <schedule>). Si acte est non-NULL
  * Il sera mis au point à l'argument `` magic cookie''
  * Passé lorsque la minuterie a été enregistrée. Il est ainsi possible
  * Pour libérer de la mémoire et éviter les fuites de mémoire. si
  * @a Une dont_call_handle_close est 0, alors le <functor> sera invoqué,
  * Qui appelle généralement le crochet de <handle_close>. Retourne 1 si
  * Annulation réussie et à 0 si le @a un timer_id n'a pas été trouvé.
   */
  virtual int cancel (long timer_id,
                      const void **act = 0,
                      int dont_call_handle_close = 1) = 0;

  /**
   * Fermer la minuterie file d'attente. Annule tous les compteurs.
   */
  virtual int close (void) = 0;

  /**
  * Renvoie l'heure actuelle de la journée. Cette méthode permet à différents
  * implémentations de la file d'attente de minuterie à utiliser une haute résolution spéciale
  * Minuteries.
   */
  virtual ACE_Time_Value gettimeofday (void) = 0;

  /**
  * Permet aux applications de contrôler la façon dont la file d'attente de minuterie obtient le temps
  * De la journée.
  * @ Deprecated Utilisez support TIME_POLICY place. voir Timer_Queue_T.h
   */
  virtual void gettimeofday (ACE_Time_Value (*gettimeofday)(void)) = 0;

  /// Déterminer le prochain événement de dépassement du délai.Retours @a max s'il existe 
  /// Pas de chronomètre en suspens ou si tous les compteurs en attente sont plus longs que max.
  /// Cette méthode pose un verrou interne car il modifie l'état interne.
  virtual ACE_Time_Value *calculate_timeout (ACE_Time_Value *max) = 0;

  /**
  * Déterminer le prochain événement de dépassement du délai. Retours @a max s'il existe
  * Pas de chronomètres en suspens ou si tous les compteurs en attente sont plus longs que max.
  * <the_timeout> Doit être un pointeur vers le stockage de la valeur de délai d'attente,
  * Et cette valeur est également renvoyée. Cette méthode ne fait pas l'acquisition d'un
  * Verrou en interne car il ne modifie pas l'état interne. Si vous avez
  * Besoin d'appeler cette méthode lorsque la file d'attente est en cours de modification
  * En même temps, cependant, vous devez vous assurer d'acquérir la <mutex()>
  * À l'extérieur avant de faire l'appel.
   */
  virtual ACE_Time_Value *calculate_timeout (ACE_Time_Value *max,
                                             ACE_Time_Value *the_timeout) = 0;

  /**
  * Retour l'heure actuelle, en utilisant la politique de moment opportun et tout
  * Minuterie obliquité défini dans les classes dérivées.
   */
  virtual ACE_Time_Value current_time() = 0;

  /// Type de itérateur.
  typedef ACE_Timer_Queue_Iterator_T<TYPE> ITERATOR;

  /// Renvoie un pointeur sur l'itérateur de ACE_Timer_Queue.
  virtual ITERATOR & iter (void) = 0;

  /// Supprime le premier noeud de la file d'attente et le renvoie
  virtual ACE_Timer_Node_T<TYPE> *remove_first (void) = 0;

  /// Lit le premier nœud de la file d'attente et le renvoie.
  virtual ACE_Timer_Node_T<TYPE> *get_first (void) = 0;

  /// Vider l'état ​​d'un objet.
  virtual void dump (void) const = 0;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Abstract_Timer_Queue.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Abstract_Timer_Queue.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_ABSTRACT_TIMER_QUEUE_H */
